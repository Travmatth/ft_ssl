/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 20:14:53 by tmatthew          #+#    #+#             */
/*   Updated: 2018/08/08 21:20:08 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

/*
** parses next option and modifies state with flag or string to be hashed
*/

static void	parse_hash_opts_handler(t_hash_state *state
	, t_digest *digest
	, char **argv
	, int *i)
{
	void	*message;

	ft_bzero(digest, sizeof(t_digest));
	if (GET_F(state->flags))
		ft_bufappend(state->digests
			, message = read_from_file(state, digest, argv, i), DIGEST_SZ);
	else if (ft_strequ("-p", argv[*i]))
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
		SET_S(state->flags);
	}
	else
		ft_bufappend(state->digests
			, message = read_from_file(state, digest, argv, i), DIGEST_SZ);
}

/*
** accepts the argc & argv arguments, parses and returns
** a struct holding an array with every string to be hashed
*/

void		*parse_hash_opts(int argc, char **argv)
{
	int				i;
	t_digest		digest;
	t_hash_state	*state;

	if (!(state = ft_memalloc(sizeof(t_hash_state)))
		|| !(state->digests = ft_bufnew(ft_memalloc(DIGEST_SZ), 0, DIGEST_SZ)))
		ft_ssl_err("error");
	i = -1;
	while (++i < argc)
		parse_hash_opts_handler(state, &digest, argv, &i);
	if ((!GET_F(state->flags) && !GET_S(state->flags)) || GET_P(state->flags))
		read_from_stdin(state, &digest);
	return ((void*)state);
}
