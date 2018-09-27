/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/13 11:01:23 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/26 22:18:25 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

t_desopt	g_desopts[] =
{
	{"des", des_null_permute_hook, des_null_permute_hook, 0},
	{"des-ecb", des_null_permute_hook, des_null_permute_hook, 0},
	{"des-cbc", des_cbc_pre_permute_hook, des_cbc_post_permute_hook, 1},
	{"des-pcbc", des_pcbc_pre_permute_hook, des_pcbc_post_permute_hook, 1},
	{"des3", des_cbc_pre_permute_hook, des_cbc_post_permute_hook, 1},
	{"des3-ecb", des_null_permute_hook, des_null_permute_hook, 0},
	{"des3-cbc", des_cbc_pre_permute_hook, des_cbc_post_permute_hook, 1},
	{"des3-pcbc", des_pcbc_pre_permute_hook, des_pcbc_post_permute_hook, 1}
};

/*
** parses next option and modifies ctx with flag or string to be hashed
*/

int			parse_param(uint8_t **param, char **argv, int *i)
{
	size_t	len;

	if (!argv[*i + 1])
		ft_ssl_usage();
	if (*param)
		free(*param);
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

int			parse_des_io(t_desctx *ctx, char **argv, int *i)
{
	int		fd;
	FILE	*fp;

	if (ft_strequ("-d", argv[*i]) || ft_strequ("-e", argv[*i]))
		argv[*i][1] == 'd' ? SET_DECRYPT(ctx->flags) : SET_ENCRYPT(ctx->flags);
	else if (ft_strequ("-i", argv[*i]))
	{
		if (!argv[*i + 1] || ERR((fd = open(argv[*i + 1], O_RDONLY))))
			ft_ssl_err("error: cannot find file");
		ctx->in_text = (uint8_t*)ft_str_from_fd(fd);
		ctx->i_len = LEN((char*)ctx->in_text, 0);
		*i += 1;
		SET_INPUT(ctx->flags);
	}
	else if (ft_strequ("-o", argv[*i]))
	{
		if (!argv[*i + 1]
			|| !(fp = fopen(argv[*i + 1], "w+")))
			ft_ssl_err("error");
		ctx->out_file = fileno(fp);
		*i += 1;
	}
	else
		return (0);
	return (1);
}

/*
** parse des inputs into struct
*/

int			parse_des_key_params(t_desctx *ctx, char **argv, int *i)
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
	else if (ft_strequ("-k", argv[*i]))
		ctx->k_len = parse_param(&ctx->key, argv, i);
	else if (ft_strequ("-p", argv[*i]))
		ctx->p_len = parse_param(&ctx->password, argv, i);
	else if (ft_strequ("-s", argv[*i]))
		ctx->s_len = parse_param(&ctx->salt, argv, i);
	else
		return (0);
	return (1);
}

/*
** Parsing functionality needed to set des struct member permute hooks
** to the functions necessary to support various block cipher chaining modes
*/

int			parse_des_mode(t_desctx *ctx, char *command)
{
	size_t	n;

	n = 0;
	while (n < 13)
	{
		if (ft_strequ(g_desopts[n].name, command))
		{
			ctx->pre_permute_chaining = g_desopts[n].pre_permute_chaining;
			ctx->post_permute_chaining = g_desopts[n].post_permute_chaining;
			if (g_desopts[n].needs_v)
				SET_NEED_V(ctx->flags);
			if (LEN(command, 0) > 3 && command[3] == '3')
				SET_TRIPLE(ctx->flags);
			return (1);
		}
		n += 1;
	}
	return (0);
}

/*
** accepts the argc & argv arguments, parses and returns
** a struct holding string to be hashed
*/

void		*parse_des_opts(int argc, char **argv)
{
	int			i;
	t_desctx	ctx;

	ft_bzero(&ctx, sizeof(t_desctx));
	if (!parse_des_mode(&ctx, argv[(i = 0)]))
		ft_ssl_cmd_err(argv[i]);
	while (++i < argc)
	{
		if (parse_des_io(&ctx, argv, &i)|| parse_des_key_params(&ctx, argv, &i))
			continue ;
		else
			ft_ssl_cmd_err(argv[i]);
	}
	verify_des_params(&ctx);
	return (ft_memcpy(ft_memalloc(sizeof(t_desctx)), &ctx, sizeof(t_desctx)));
}
