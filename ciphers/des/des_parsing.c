/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/13 11:01:23 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/23 18:58:36 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

/*
** parses next option and modifies ctx with flag or string to be hashed
*/

int		parse_param(char *opt, uint8_t **param, char **argv, int *i)
{
	size_t	len;

	if (!ft_strequ(opt, argv[*i]))
		return (0);
	if (!argv[*i + 1])
		ft_ssl_usage();
	if (!(*param = (uint8_t*)ft_strdup(argv[*i + 1])))
		ft_ssl_err("error");
	*i += 1;
	len = LEN((char*)*param, 0);
	if (!len)
		ft_ssl_err("error");
	return (len);
}

/*
** parse des inputs into struct
*/

int		parse_des_io(t_desctx *ctx, char **argv, int *i)
{
	int		fd;
	FILE	*fp;

	if (ft_strequ("-i", argv[*i]))
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
		if (!argv[*i + 1]
			|| !(fp = fopen(argv[*i + 1], "w+")))
			ft_ssl_err("error");
		ctx->out_file = fileno(fp);
		*i += 1;
		return (1);
	}
	return (0);
}

/*
** parse des inputs into struct
*/

int		parse_des_key_params(t_desctx *ctx, char **argv, int *i)
{
	if (ft_strequ("-v", argv[*i]))
	{
		if (!argv[*i + 1])
			ft_ssl_err("error");
		if (!(ft_htouint64((uint8_t*)argv[*i + 1], &ctx->init_vector)))
			ft_ssl_err("error: invalid init vector");
		*i += 1;
		SET_V(ctx->flags);
		return (1);
	}
	else if (ft_strequ("-a", argv[*i]))
	{
		SET_A(ctx->flags);
		return (1);
	}
	else if ((ctx->k_len = parse_param("-k", &ctx->key, argv, i)))
		return (1);
	else if ((ctx->p_len = parse_param("-p", &ctx->password, argv, i)))
		return (1);
	else if ((ctx->s_len = parse_param("-s", &ctx->salt, argv, i)))
		return (1);
	return (0);
}

/*
** parse des inputs into struct
*/

int		parse_des_params(t_desctx *ctx, char **argv, int *i)
{
	if (parse_des_mode(ctx, argv, i))
		return (1);
	if (ft_strequ("-d", argv[*i]) || ft_strequ("-e", argv[*i]))
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

void	*parse_des_opts(int argc, char **argv)
{
	int			i;
	t_desctx	ctx;

	i = -1;
	ft_bzero(&ctx, sizeof(t_desctx));
	while (++i < argc)
	{
		if (parse_des_params(&ctx, argv, &i) || parse_des_io(&ctx, argv, &i)
			|| parse_des_key_params(&ctx, argv, &i))
			continue ;
		else
			ft_ssl_cmd_err(argv[i]);
	}
	verify_des_params(&ctx);
	return (ft_memcpy(ft_memalloc(sizeof(t_desctx)), &ctx, sizeof(t_desctx)));
}
