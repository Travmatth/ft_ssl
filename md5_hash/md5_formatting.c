/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_formatting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/10 17:23:04 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/19 20:36:18 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ssl.h"

# define MD5_DIGEST(state, i) ((t_digest*)((char*)state->digests->buf + i))

void	print_md5_state(t_md5_state *state)
{
	size_t	i;
	t_digest *d;

	i = 0;
	if (GET_P(state->flags))
	{
		d = MD5_DIGEST(state, i);
		ft_putstr(d->pre_image);
		ft_putendl(MD5_DIGEST(state, i)->hash_value);
		i += sizeof(t_digest);
	}
	while (i < state->digests->current)
	{
		if (GET_Q(state->flags) && MD5_DIGEST(state, i)->type == FROM_FILE)
			ft_putendl(MD5_DIGEST(state, i)->hash_value);
		else if (GET_R(state->flags) && MD5_DIGEST(state, i)->type == FROM_FILE)
			ft_printf("%s %s\n"
				, MD5_DIGEST(state, i)->hash_value
				, MD5_DIGEST(state, i)->file_name);
		else if (MD5_DIGEST(state, i)->type == FROM_FILE)
			ft_printf("MD5 (%s) = %s\n"
				, MD5_DIGEST(state, i)->file_name
				, MD5_DIGEST(state, i)->hash_value);
		else if (MD5_DIGEST(state, i)->type == FROM_STRING)
			ft_printf("MD5 (\"%s\") = %s\n"
				, MD5_DIGEST(state, i)->pre_image
				, MD5_DIGEST(state, i)->hash_value);
		// else
		// 	ft_putendl(MD5_DIGEST(state, i)->hash_value);
		i += sizeof(t_digest);
	}
}
