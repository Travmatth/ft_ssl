/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_opts.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/03 18:50:26 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/07 16:08:07 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ssl.h"

t_buf		*ft_bufaddspace(t_buf *b, size_t i)
{
	char	*tmp;

	if (!b)
		return (NULL);
	if (!i)
		return (b);
	tmp = ft_realloc(b->buf, b->total + i);
	b->buf = tmp;
	b->total += i;
}

char	*ft_strnjoin(char const *s1, char const *s2, size_t i)
{
	char	*u_str;
	size_t	i;
	size_t	j;
	size_t	s1_len;
	size_t	s2_len;

	if (!s1 || !s2)
		return (NULL);
	i = -1;
	j = -1;
	s1_len = LEN(s1, 0);
	s2_len = LEN(s2, 0);
	if (!(u_str = ft_strnew((s1_len + s2_len))))
		return (NULL);
	while (++i < s1_len)
		*(u_str + i) = *(s1 + i);
	while (++j < s2_len && j < i)
		*(u_str + i++) = *(s2 + j);
	return (u_str);
}

char		*ft_str_from_fd(int fd)
{
	size_t	i;
	size_t	bytes;
	char	buf[BUFF_SIZE];
	char	*string;
	char	*tmp;

	string = ft_memalloc(1);
	while ((bytes = read(fd, buf, BUFF_SIZE)) != 0)
	{
		tmp = ft_strnjoin(string, buf, BUFF_SIZE);
		free(string);
		string = tmp;
	}
	return (string);
}

static void	read_from_stdin(t_digest *input, t_msg digest, int file)
{
	size_t	i;
	char	*string;
	char	*new_offset;

	if (file && !GET_P(input->flags))
		return ;
	digest.msg = ft_str_from_fd(STDIN);
	digest.type = FROM_STDIN;
	input->digests = ft_bufaddspace(input->digests, sizeof(t_msg));
	new_offset = (char*)input->digests + sizeof(t_msg);
	ft_memmove((void*)new_offset, input->digests, sizeof(t_msg));
	ft_memcpy(input->digests, &digest, sizeof(t_msg));
}

t_digest	*parse_opts(int argc, char **argv)
{
	int			file;
	int			i;
	int			fd;
	size_t		len;
	t_digest	*input;
	t_msg		digest;
	char		**args;

	if (!(input = ft_memalloc(sizeof(t_digest)))
		|| !(input->digests = ft_bufnew(ft_memalloc(1), 0, sizeof(t_msg))))
		ft_ssl_err("error");
	i = -1;
	args = ft_strsplit(*argv, ' ');
	while (++i < argc)
	{
		ft_bzero(&digest, sizeof(t_msg));
		if (ft_strequ("-p", args[i]))
		{
			SET_P(input->flags);
			free(args[i]);
			ft_bufappend(input->digests, &digest, sizeof(t_msg));
		}
		else if (ft_strequ("-q", args[i]))
		{
			SET_Q(input->flags);
			free(args[i]);
			ft_bufappend(input->digests, &digest, sizeof(t_msg));
		}
		else if (ft_strequ("-r", args[i]))
		{
			SET_R(input->flags);
			free(args[i]);
			ft_bufappend(input->digests, &digest, sizeof(t_msg));
		}
		else if (ft_strequ("-s", args[i]))
		{
			if (!args[1])
				ft_ssl_err("error");
			len = LEN((char*)args[1], 0);
			digest.msg = args[i + 1];
			digest.type = FROM_STRING;
			free(args[i]);
			ft_bufappend(input->digests, &digest, sizeof(t_msg));
		}
		else
		{
			if (ERR(fd = open(args[i], O_RDONLY)))
				ft_ssl_err(FT_MD5_NO_SUCH_FILE);
			digest.msg = ft_str_from_fd(fd);
			close(fd);
			digest.type = FROM_FILE;
			free(args[i]);
			file = TRUE;
		}
	}
	free(args);
	read_from_stdin(input, digest, file)
	return (input);
}
