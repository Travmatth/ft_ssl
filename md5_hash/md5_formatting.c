/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5_formatting.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/10 17:23:04 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/20 14:40:36 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ssl.h"

/*
** print hash in accordance with formatting options
*/

void	md5_hash_print_handler(char flags[], t_digest *d, size_t *i)
{
	if (d->type == NO_INPUT)
		ft_printf(FT_MD5_NO_SUCH_FILE, d->file_name);
	else if (GET_Q(flags))
		ft_putendl(d->hash_value);
	else if (GET_R(flags) && d->type == FROM_FILE)
		ft_printf("%s %s\n", d->hash_value, d->file_name);
	else if (GET_R(flags) && d->type == FROM_STRING)
		ft_printf("%s \"%s\"\n", d->hash_value, d->pre_image);
	else if (d->type == FROM_FILE)
		ft_printf("MD5 (%s) = %s\n", d->file_name, d->hash_value);
	else if (d->type == FROM_STRING)
		ft_printf("MD5 (\"%s\") = %s\n", d->pre_image, d->hash_value);
	free(d->pre_image);
	free(d->hash_value);
	*i += sizeof(t_digest);
}

/*
** iterate over & print array of computed hashes according to formatting options
*/

void	print_md5_state(t_md5_state *state)
{
	size_t		i;
	t_digest	*d;

	i = 0;
	if (GET_P(state->flags))
	{
		d = MD5_DIGEST(state, i);
		ft_putstr(d->pre_image);
		ft_putendl(d->hash_value);
		free(d->pre_image);
		free(d->hash_value);
		i += sizeof(t_digest);
	}
	while (i < state->digests->current)
		md5_hash_print_handler(state->flags, MD5_DIGEST(state, i), &i);
}
