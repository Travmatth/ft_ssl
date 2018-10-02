/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/08 16:47:30 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/01 20:00:09 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

/*
** attempt to read string from file given by user
*/

void		read_from_stdin(t_hash_state *state, t_digest *digest)
{
	void	*tmp;

	ft_str_from_fd(STDIN, &digest->pre_image);
	digest->type = FROM_STDIN;
	tmp = ft_memdup(state->digests->buf, state->digests->current);
	state->digests = ft_bufaddspace(state->digests, sizeof(t_digest));
	ft_memcpy(state->digests->buf, (void*)digest, sizeof(t_digest));
	ft_memcpy((char*)state->digests->buf + sizeof(t_digest)
			, tmp
			, state->digests->current);
	state->digests->current += sizeof(t_digest);
	free(tmp);
}

/*
** attempt to read string from file given by user
*/

void		*read_from_file(t_hash_state *state
							, t_digest *digest
							, char **argv, int *i)
{
	int			fd;

	SET_F(state->flags);
	digest->file_name = argv[*i];
	if (ERR((fd = open(argv[*i], O_RDONLY))))
	{
		digest->type = NO_INPUT;
		return (digest);
	}
	ft_str_from_fd(fd, &digest->pre_image);
	close(fd);
	digest->type = FROM_FILE;
	return (digest);
}
