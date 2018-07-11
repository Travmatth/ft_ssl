/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/30 20:13:01 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/10 18:27:29 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ssl.h"

static char		*md5(t_digest *digest)
{
	(void)digest;
	return (NULL);
}

t_md5_state	*make_md5_hash(t_md5_state *state)
{
	size_t		i;
	size_t		total;
	t_digest	*digest;
	char		*hash;

	i = 0;
	total = state->digests->current;
	while (i <= total - sizeof(t_digest))
	{
		digest = (t_digest*)&((char*)state->digests->buf)[i];
		hash = md5(digest);
		ft_memcpy(digest->hash, hash, MD5_HASH_SIZE);
		i += sizeof(t_digest);
	}
	return (state);
}
