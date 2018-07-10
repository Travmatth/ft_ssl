/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_opts.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/03 18:50:26 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/10 11:57:10 by tmatthew         ###   ########.fr       */
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
** joins the second string with the first up to the first n bytes
** and returns this in a freshly allocated string
*/

char	*ft_strnjoin(char const *s1, char const *s2, size_t n)
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
	if (!(u_str = ft_strnew(s1_len + LESSER(n, s2_len))))
		return (NULL);
	while (++i < s1_len)
		*(u_str + i) = *(s1 + i);
	while (++j < s2_len && j < n)
		*(u_str + i++) = *(s2 + j);
	return (u_str);
}

/*
** reads a given fd and returns a string with its contents
*/

char		*ft_str_from_fd(int fd)
{
	size_t	bytes;
	char	buf[BUFF_SIZE];
	char	*string;
	char	*tmp;

	bytes = 0;
	string = ft_memalloc(1);
	while ((bytes = read(fd, buf, BUFF_SIZE)) != 0)
	{
		tmp = ft_strnjoin(string, buf, bytes);
		free(string);
		string = tmp;
	}
	return (string);
}

/*
** attempt to read string from file given by user
*/

static void	read_from_stdin(t_state *state, t_digest *digest)
{
	char	*new_offset;

	digest->msg = ft_str_from_fd(STDIN);
	digest->type = FROM_STDIN;
	state->digests = ft_bufaddspace(state->digests, DIGEST_SZ);
	new_offset = (char*)state->digests->buf + DIGEST_SZ;
	ft_memmove((void*)new_offset, state->digests->buf, state->digests->current);
	ft_memcpy(state->digests, &digest, DIGEST_SZ);
}

/*
** attempt to read string from file given by user
*/

static void	read_from_file(t_digest *digest, char **argv, int *i)
{
	int			fd;

	if (ERR((fd = open(argv[*i], O_RDONLY))))
		ft_ssl_err(FT_MD5_NO_SUCH_FILE);
	digest->msg = ft_str_from_fd(fd);
	close(fd);
	digest->type = FROM_FILE;
	SET_F(state->flags)
	return (digest);
}

/*
** parses next option and modifies state with flag or string to be hashed
*/

static void	parse_opts_handler(t_state *state
								, t_digest *digest
								, char **argv
								, int &i)
{
	char	*str;

	ft_bzero(&digest, DIGEST_SZ);
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
		digest->msg = ft_strdup(argv[*i + 1]);
		digest->type = FROM_STRING;
		ft_bufappend(state->digests, digest, DIGEST_SZ);
		*i += 1;
	}
	else
	{
		str = read_from_file(digest, argv, file);
		ft_bufappend(state->digests, str, DIGEST_SZ);
	}
}

/*
** accepts the argc & argv arguments, parses and returns
** a struct holding an array with every string to be hashed
*/

t_state	*parse_opts(int argc, char **argv)
{
	int			file;
	int			i;
	t_state	*state;
	t_digest		digest;

	if (!(state = ft_memalloc(sizeof(t_state)))
		|| !(state->digests = ft_bufnew(ft_memalloc(DIGEST_SZ), 0, DIGEST_SZ)))
		ft_ssl_err("error");
	i = -1;
	while (++i < argc - 1)
		parse_opts_handler(state, &digest, argv, &i);
	if (GET_F(state->flags) || GET_P(state->flags))
		read_from_stdin(state, &digest);
	return (state);
}
