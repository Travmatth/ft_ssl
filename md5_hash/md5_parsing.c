/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/11 16:54:53 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/18 15:45:33 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ssl.h"

/*
** expand buf by given size, free previous buf
*/

t_buf		*ft_bufaddspace(t_buf *b, size_t i)
{
	char	*tmp;

	if (!b)
		return (NULL);
	if (!i)
		return (b);
	tmp = ft_memalloc(b->total + i);
	ft_memcpy(tmp, b->buf, b->current);
	free(b->buf);
	b->buf = tmp;
	b->total += i;
	return (b);
}

/*
** reads a given fd and returns a string with its contents
*/

char		*ft_str_from_fd(int fd)
{
	size_t	len;
	size_t	bytes;
	char	*tmp;
	char	*string;
	char	buf[BUFF_SIZE];

	bytes = 0;
	string = NULL;
	while ((bytes = read(fd, buf, BUFF_SIZE)) != 0)
	{
		if (string)
		{
			len = LEN(string, 0);
			if (!(tmp = ft_strnew(len + bytes)))
				ft_ssl_err("error");
			ft_memcpy(tmp, string, len);
			ft_memcpy(tmp + len, buf, bytes);
			free(string);
			string = tmp;
		}
		else
			string = ft_strndup(buf, bytes);
	}
	return (string);
}

/*
** attempt to read string from file given by user
*/

static void	read_from_stdin(t_md5_state *state, t_digest *digest)
{
	char	*new_offset;

	digest->pre_image = ft_str_from_fd(STDIN);
	digest->type = FROM_STDIN;
	state->digests = ft_bufaddspace(state->digests, sizeof(t_digest));
	new_offset = (char*)state->digests->buf + sizeof(t_digest);
	ft_memmove((void*)new_offset, state->digests->buf, state->digests->current);
	ft_memcpy(state->digests->buf, (void*)digest, sizeof(t_digest));
	state->digests->current += sizeof(t_digest);
}

/*
** attempt to read string from file given by user
*/

static void	*read_from_file(t_md5_state *state, t_digest *digest, char **argv, int *i)
{
	int			fd;

	if (ERR((fd = open(argv[*i], O_RDONLY))))
		ft_ssl_err(FT_MD5_NO_SUCH_FILE);
	digest->pre_image = ft_str_from_fd(fd);
	close(fd);
	digest->type = FROM_FILE;
	digest->file_name = argv[*i];
	SET_F(state->flags);
	return (digest);
}

/*
** parses next option and modifies state with flag or string to be hashed
*/

static void	parse_md5_opts_handler(t_md5_state *state, t_digest *digest, char **argv, int *i)
{
	void	*message;

	ft_bzero(digest, sizeof(t_digest));
	if (ft_strequ("-p", argv[*i]))
		SET_P(state->flags);
	else if (ft_strequ("-q", argv[*i]))
		SET_Q(state->flags);
	else if (ft_strequ("-r", argv[*i]))
		SET_R(state->flags);
	else if (ft_strequ("-s", argv[*i]))
	{
		if (!argv[*i + 1])
			ft_ssl_err("error");
		digest->pre_image = ft_strdup(argv[*i + 1]);
		digest->type = FROM_STRING;
		ft_bufappend(state->digests, digest, DIGEST_SZ);
		*i += 1;
	}
	else
	{
		message = read_from_file(state, digest, argv, i);
		ft_bufappend(state->digests, message, DIGEST_SZ);
	}
}

/*
** accepts the argc & argv arguments, parses and returns
** a struct holding an array with every string to be hashed
*/

void		*parse_md5_opts(int argc, char **argv)
{
	int			i;
	t_digest	digest;
	t_md5_state	*state;

	if (!(state = ft_memalloc(sizeof(t_md5_state)))
		|| !(state->digests = ft_bufnew(ft_memalloc(DIGEST_SZ), 0, DIGEST_SZ)))
		ft_ssl_err("error");
	i = -1;
	while (++i < argc - 1)
		parse_md5_opts_handler(state, &digest, argv, &i);
	if (!GET_F(state->flags) || GET_P(state->flags))
		read_from_stdin(state, &digest);
	return ((void*)state);
}
