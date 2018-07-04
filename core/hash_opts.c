/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_opts.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/03 18:50:26 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/03 20:27:28 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ssl.h"

static void	read_from_stdin(t_digest *input)
{
	char	buf[BUFF_SIZE];
	t_msg	digest;
	t_buf	b;
	int		bytes;

	ft_bzero(buf);
	while ((bytes = read(STDIN, buf, BUFF_SIZE)))
		ft_bufappend(b, buf, bytes);
	ft_memcpy(input->digests[0], b, sizeof(t_buf));
}

t_digest	*parse_opts(int argc, char *argv)
{
	int			i;
	size_t		len;
	t_digest	*input;
	t_msg		digest;

	if (!(input = ft_memalloc(sizeof(t_digest)))
		|| !(input->digests = ft_bufnew(ft_memalloc(0), 0, sizeof(t_msg))))
		ft_ssl_err("error");
	i = 0;
	read_from_stdin(input);
	while (i < argc)
	{
		ft_bzero(&digest, sizeof(t_msg));
		if (ft_strequ("-p", argv[0]))
		{
			SET_P(input->flags);
			*argv = argv[1];
		}
		else if (ft_strequ("-q", argv[0]))
		{
			SET_Q(input->flags);
			*argv = argv[1];
		}
		else if (ft_strequ("-r", argv[0]))
		{
			SET_R(input->flags);
			*argv = argv[1];
		}
		else if (ft_strequ("-s", argv[0]))
		{
			if (!argv[1])
				ft_ssl_err("error");
			len = LEN(argv[1], 0);
			input->digests[i].msg = ft_strsub(argv[1]
				, *argv[1] == '"' ? 1 : 0
				, len - (*argv[1] == '"' ? 1 : 0 ))
			input->digests[i].type = STRING;
			*argv = argv[2];
		}
		ft_bufappend(input->digests, digest, sizeof(t_msg))
		i += 1;
	}
}
