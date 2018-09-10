/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b64_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/08 21:12:21 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/09 20:58:33 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

/*
** Implements the following flags for base64:
** • -d, decode mode
** • -e, encode mode (default)
** • -i, input file
** • -o, output file
** If flags are not provided, reads/writes from/to the console.
** >
** > echo toto | ./ft_ssl base64 -e
** dG90bwo=
** >
** > echo dG90bwo= | ./ft_ssl base64 -d
** toto
** > echo "d G9 bwo =" | ./ft_ssl base64 -d
** toto
** >
** > echo foobar | ./ft_ssl base64 -e
** Zm9vYmFyCg==
** >
** > echo Zm9vYmFyCg== | base64 -D
** foobar
*/

/*
** parses next option and modifies state with flag or string to be hashed
*/

static void	parse_b64_opts_handler(t_base64 *ctx, char **argv, int *i)
{
	int			fd;

	if ((ft_strequ("-d", argv[*i]) || ft_strequ("-D", argv[*i])))
		SET_D(ctx->mode);
	else if (ft_strequ("-e", argv[*i]) || ft_strequ("-D", argv[*i]))
		SET_E(ctx->mode);
	else if (ft_strequ("-i", argv[*i]))
	{
		SET_I(ctx->mode);
		if (!argv[*i + 1])
			ft_ssl_err("error");
		if (ERR((fd = open(argv[*i++ + 1], O_RDONLY))))
			ft_ssl_err("base64: no such file found");
		ctx->in = (unsigned char*)ft_str_from_fd(fd);
		ctx->in_len = LEN((char*)ctx->in, 0);
	}
	else if (ft_strequ("-o", argv[*i]))
	{
		SET_O(ctx->mode);
		if (!argv[*i + 1])
			ft_ssl_err("error");
		if (!ERR((fd = open(argv[*i++ + 1], O_RDWR))))
			ctx->fd = fd;
		else
			ft_ssl_err("error");
	}
}

/*
** accepts the argc & argv arguments, parses and returns
** a struct holding string to be hashed
*/

void		*parse_b64_opts(int argc, char **argv)
{
	int			i;
	t_base64	ctx;
	t_base64	*new;


	i = -1;
	ft_bzero(&ctx, sizeof(t_base64));
	while (++i < argc)
		parse_b64_opts_handler(&ctx, argv, &i);
	if (!GET_I(ctx.mode))
		ctx.in = (unsigned char*)ft_str_from_fd(STDIN);
	if (!(new = ft_memalloc(sizeof(t_base64))))
		ft_ssl_err("error");
	return (ft_memcpy(new, &ctx, sizeof(t_base64)));
}
