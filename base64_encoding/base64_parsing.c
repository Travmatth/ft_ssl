/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64_parsing.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/08 21:12:21 by tmatthew          #+#    #+#             */
/*   Updated: 2018/08/10 11:57:07 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

/*
** > echo toto | ./ft_ssl base64 -e
** dG90bwo=
** >
** > echo dG90bwo= | ./ft_ssl base64 -d
** toto
** > echo "d G9 bwo =" | ./ft_ssl base64 -d
** toto
** Your program must be able to encode and decode to the existing base64 character
** set, so it will cooperate with existing technologies.
** > echo foobar | ./ft_ssl base64 -e
** Zm9vYmFyCg==
** >
** > echo Zm9vYmFyCg== | base64 -D
** foobar
** You must implement the following flags for base64:
** • -d, decode mode
** • -e, encode mode (default)
** • -i, input file
** • -o, output file
** If flags are not provided, be prepared to read/write from/to the console.
*/

/*
** parses next option and modifies state with flag or string to be hashed
*/

static void	parse_base64_opts_handler(t_base64 *message
	, char **argv
	, int *i)
{
	int			fd;

	if ((ft_strequ("-d", argv[*i]) || ft_strequ("-D", argv[*i])))
		SET_D(message->mode);
	else if (ft_strequ("-e", argv[*i]) || ft_strequ("-D", argv[*i]))
		SET_E(message->mode);
	else if (ft_strequ("-i", argv[*i]))
	{
		SET_I(message->mode);
		if (!argv[*i + 1])
			ft_ssl_err("error");
		if (message->message)
			ft_strdel(&message->message);
		message->input = ft_strdup(argv[*i + 1]);
		*i += 1;
		if (!ERR((fd = open(message->input, O_RDONLY))))
			message->message = ft_str_from_fd(fd);
		else
			ft_ssl_err("base64: no such file found");
	}
	else if (ft_strequ("-o", argv[*i]))
	{
		SET_O(message->mode);
		if (!argv[*i + 1])
			ft_ssl_err("error");
		message->output = ft_strdup(argv[*i + 1]);
		*i += 1;
	}
}

/*
** accepts the argc & argv arguments, parses and returns
** a struct holding string to be hashed
*/

void		*parse_base64_opts(int argc, char **argv)
{
	int			i;
	t_base64	message;


	i = -1;
	ft_bzero(&message, sizeof(t_base64));
	while (++i < argc)
		parse_base64_opts_handler(&message, argv, &i);
	if (!GET_I(message.mode))
		message.message = ft_str_from_fd(STDIN);
	return (ft_memcpy(ft_memalloc(sizeof(t_base64))
		, &message
		, sizeof(t_base64)));
}
