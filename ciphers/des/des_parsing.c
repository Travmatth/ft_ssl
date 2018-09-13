/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/13 11:01:23 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/12 19:19:31 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

/*
** parses next option and modifies ctx with flag or string to be hashed
*/

int		parse_param(char *opt, uint8_t **param, char **argv, int *i)
{
	if (!ft_strequ(opt, argv[*i]))
		return (0);
	if (!(*param = (uint8_t*)ft_strdup(argv[*i + 1])))
		ft_ssl_err("error");
	*i += 1;
	return (LEN((char*)*param, 0));
}

int		parse_des_io(t_desctx *ctx, char **argv, int *i)
{
	int		fd;

	if (ft_strequ("-a", argv[*i]))
	{
		SET_A(ctx->flags);
		return (1);
	}
	else if (ft_strequ("-i", argv[*i]))
	{
		if (!argv[*i + 1] || ERR((fd = open(argv[*i + 1], O_RDONLY))))
			ft_ssl_err("error: cannot find file");
		ctx->in_text = (uint8_t*)ft_str_from_fd(fd);
		ctx->i_len = LEN((char*)ctx->in_text, 0);
		*i += 1;
		SET_INPUT(ctx->flags);
		return (1);
	}
	else if (ft_strequ("-o", argv[*i]))
	{
		if (!argv[*i + 1] || ERR((ctx->out_file = open(argv[*i + 1], O_WRONLY))))
			ft_ssl_err("error");
		*i += 1;
		return (1);
	}
	return (0);
}

int		parse_des_key_params(t_desctx *ctx, char **argv, int *i)
{
	if (ft_strequ("-v", argv[*i]))
	{
		if (!argv[*i + 1])
			ft_ssl_err("error");
		if (!(ft_htouint64((uint8_t*)argv[*i + 1], &ctx->init_vector)))
			ft_ssl_err("error: invalid init vector");
		*i += 1;
		return (1);
	}
	else if ((ctx->k_len = parse_param("-k", &ctx->key, argv, i)))
		return (1);
	else if ((ctx->i_len = parse_param("-p", &ctx->password, argv, i)))
		return (1);
	else if ((ctx->s_len = parse_param("-s", &ctx->salt, argv, i)))
		return (1);
	return (0);
}

static int		parse_des_params(t_desctx *ctx, char **argv, int *i)
{
	if (ft_strequ("des-ecb", argv[*i]) || ft_strequ("des", argv[*i]))
	{
		ctx->pre_permute_chaining = des_null_permute_hook;
		ctx->post_permute_chaining = des_null_permute_hook;
		return (1);
	}
	else if (ft_strequ("des-cbc", argv[*i]))
	{
		ctx->pre_permute_chaining = des_cbc_pre_permute_hook;
		ctx->post_permute_chaining = des_cbc_post_permute_hook;
		return (1);
	}
	else if (ft_strequ("-d", argv[*i]) || ft_strequ("-e", argv[*i]))
	{
		argv[*i][1] == 'd' ? SET_DECRYPT(ctx->flags) : SET_ENCRYPT(ctx->flags);
		return (1);
	}
	return (0);
}

/*
** accepts the argc & argv arguments, parses and returns
** a struct holding string to be hashed
*/

void			*parse_des_opts(int argc, char **argv)
{
	int			i;
	t_desctx	ctx;
	void		*new;

	i = -1;
	ft_bzero(&ctx, sizeof(t_desctx));
	while (++i < argc)
	{
		if (parse_des_params(&ctx, argv, &i)
			|| parse_des_io(&ctx, argv, &i)
			|| parse_des_key_params(&ctx, argv, &i))
			continue ;
		else
			ft_ssl_err("error unknown command");
	}
	if (!GET_DECRYPT(ctx.flags) && !GET_ENCRYPT(ctx.flags))
		SET_ENCRYPT(ctx.flags);
	if (!ctx.out_file)
		ctx.out_file = STDOUT;
	if (!GET_INPUT(ctx.flags))
	{
		ctx.in_text = (uint8_t*)ft_str_from_fd(STDIN);
		ctx.i_len = LEN((char*)ctx.in_text, 0);
	}
	if (!(new = ft_memalloc(sizeof(t_desctx))))
		ft_ssl_err("error");
	return (ft_memcpy(new, &ctx, sizeof(t_desctx)));
}
