/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_printing.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 20:35:57 by tmatthew          #+#    #+#             */
/*   Updated: 2018/08/08 21:20:13 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

/*
** convert given hex values into into string
*/

char			*from_hex_hash(char *output
							, unsigned char *hash_value
							, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		ft_snprintf(output + (i * 2), 3, "%02x\n", hash_value[i]);
		i += 1;
	}
	output[i * 2] = '\0';
	free(hash_value);
	return (ft_strdup(output));
}

/*
** print hash in accordance with formatting options
*/

void			hash_print_handler(char *hash
								, char flags[]
								, t_digest *d
								, size_t *i)
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
		ft_printf("%s (%s) = %s\n", hash, d->file_name, d->hash_value);
	else if (d->type == FROM_STRING)
		ft_printf("%s (\"%s\") = %s\n", hash, d->pre_image, d->hash_value);
	else if (d->type == FROM_STDIN)
		ft_printf("%s\n", d->hash_value);
	free(d->pre_image);
	free(d->hash_value);
	*i += sizeof(t_digest);
}

/*
** iterate over & print array of computed hashes according to formatting options
*/

void			print_hash_state(char *hash, t_hash_state *state)
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
		hash_print_handler(hash, state->flags, MD5_DIGEST(state, i), &i);
}
