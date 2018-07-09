/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_opts.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/03 18:50:26 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/09 15:02:33 by tmatthew         ###   ########.fr       */
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
	// if (b->current = )
	tmp = ft_realloc(b->buf, b->total + i);
	b->buf = tmp;
	b->total += i;
	return (b);
}

# define LESSER(x, y) (x < y ? x : y)

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

static void	read_from_stdin(t_digest *input, t_msg digest)
{
	char	*new_offset;

	digest.msg = ft_str_from_fd(STDIN);
	digest.type = FROM_STDIN;
	input->digests = ft_bufaddspace(input->digests, sizeof(t_msg));
	new_offset = (char*)input->digests + sizeof(t_msg);
	ft_memmove((void*)new_offset, input->digests->buf, input->digests->current);
	ft_memcpy(input->digests, &digest, sizeof(t_msg));
}

/*
** we always read first from the stdin
** 	write to stdout w newline
** if only -p flag is given
** 	write message to stdout w newline
** 	write hash to stdout w newline
** if -q is given
** 	only echo the hash w newline, nothing else
** 		*if -p, still echo the given line
** if -r is given for file
** 	echo the hash
** 	followed by one space
** 	followed by the filename followed by newline
** if not an option, check if filename
** 	if file
** 		read file and write content hash to stdout w following syntax:
** 			`MD5 (<name>) = <hash>`
** 	else
** 		print: `ft_ssl: md5: <file> no such file or directory`
** if -s
** 	hash following string w syntax:
** 		`MD5 (<string>) = <hash>`
** order of evaluation:
** 	handle stdin
** 	file || string, whichever comes first
*/

t_digest	*parse_opts(int argc, char **argv)
{
	int			file;
	int			i;
	int			fd;
	size_t		len;
	t_digest	*input;
	t_msg		digest;
	// char		**args;

	if (!(input = ft_memalloc(sizeof(t_digest)))
		|| !(input->digests = ft_bufnew(ft_memalloc(sizeof(t_msg)), 0, sizeof(t_msg))))
		ft_ssl_err("error");
	file = FALSE;
	i = -1;
	// args = ft_strsplit(*argv, ' ');
	while (++i < argc)
	{
		ft_bzero(&digest, sizeof(t_msg));
		// if only -p flag is given
		// 	write message to stdout w newline
		// 	write hash to stdout w newline
		if (ft_strequ("-p", argv[i]))
		{
			SET_P(input->flags);
			// free(args[i]);
			// ft_bufappend(input->digests, &digest, sizeof(t_msg));
		}
		// if -q is given
		// 	only echo the hash w newline, nothing else
		// 		*if -p, still echo the given line
		else if (ft_strequ("-q", argv[i]))
		{
			SET_Q(input->flags);
			// free(args[i]);
			// ft_bufappend(input->digests, &digest, sizeof(t_msg));
		}
		// if -r is given for file
		// 	echo the hash
		// 	followed by one space
		// 	followed by the filename followed by newline
		else if (ft_strequ("-r", argv[i]))
		{
			SET_R(input->flags);
			// free(args[i]);
			// ft_bufappend(input->digests, &digest, sizeof(t_msg));
		}
		// if -s
		// 	hash following string w syntax:
		// 		`MD5 (<string>) = <hash>`
		else if (ft_strequ("-s", argv[i]))
		{
			if (!argv[1])
				ft_ssl_err("error");
			len = LEN((char*)argv[1], 0);
			digest.msg = argv[i + 1];
			digest.type = FROM_STRING;
			// free(args[i]);
			ft_bufappend(input->digests, &digest, sizeof(t_msg));
		}
		else
		{
			if (ERR(fd = open(argv[i], O_RDONLY)))
				ft_ssl_err(FT_MD5_NO_SUCH_FILE);
			digest.msg = ft_str_from_fd(fd);
			close(fd);
			digest.type = FROM_FILE;
			// free(args[i]);
			file = TRUE;
		}
	}
	// free(args);
	if (!file || GET_P(input->flags))
		read_from_stdin(input, digest);
	return (input);
}
