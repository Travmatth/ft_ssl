/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b64_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/08 21:12:21 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/24 12:01:49 by tmatthew         ###   ########.fr       */
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

static void	parse_b64_opts_handler(t_b64 *ctx, char **argv, int *i)
{
	int		fd;
	FILE	*fp;

	if ((ft_strequ("-d", argv[*i]) || ft_strequ("-D", argv[*i])))
		SB64_DECODE(ctx->mode);
	else if (ft_strequ("-e", argv[*i]) || ft_strequ("-D", argv[*i]))
		SB64_ENCODE(ctx->mode);
	else if (ft_strequ("-i", argv[*i]))
	{
		SB64_IN(ctx->mode);
		if (!argv[*i + 1] || ERR((fd = open(argv[*i++ + 1], O_RDONLY))))
			ft_ssl_err("base64: no such file found");
		ctx->in = (unsigned char*)ft_str_from_fd(fd);
		ctx->in_len = LEN((char*)ctx->in, 0);
	}
	else if (ft_strequ("-o", argv[*i]))
	{
		SB64_OUT(ctx->mode);
		if (!argv[*i + 1]
			|| !(fp = fopen(argv[*i++ + 1], "w+")))
			ft_ssl_err("error");
		ctx->fd = fileno(fp);
	}
}

/*
** accepts the argc & argv arguments, parses and returns
** a struct holding string to be hashed
*/

void		*parse_b64_opts(int argc, char **argv)
{
	int			i;
	t_b64		ctx;
	t_b64		*new;

	i = -1;
	ft_bzero(&ctx, sizeof(t_b64));
	while (++i < argc)
		parse_b64_opts_handler(&ctx, argv, &i);
	if (!GB64_IN(ctx.mode))
	{
		ctx.in = (unsigned char*)ft_str_from_fd(STDIN);
		ctx.in_len = LEN((char*)ctx.in, 0);
	}
	if (!GB64_ENCODE(ctx.mode) && !GB64_DECODE(ctx.mode))
		SB64_ENCODE(ctx.mode);
	if (!ctx.fd)
		ctx.fd = STDOUT;
	if (!(new = ft_memalloc(sizeof(t_b64))))
		ft_ssl_err("error");
	return (ft_memcpy(new, &ctx, sizeof(t_b64)));
}
