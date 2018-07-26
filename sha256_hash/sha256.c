/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 12:41:53 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/26 14:30:56 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ssl.h"
#include "sha256.h"
#include "sha256_constants.h"

void			sha256_round(int i, uint32_t *message_schedule, uint32_t *vars)
{
	uint32_t	tmp[2];

	tmp[0] = vars[H]
		+ EP1(vars[E])
		+ CH(vars[E], vars[F], vars[G])
		+ K[i]
		+ W[i];
	tmp[1] = EP0(vars[A]) + MAJ(vars[A], vars[B], vars[C]);
	vars[H] = vars[G];
	vars[G] = vars[F];
	vars[F] = vars[E];
	vars[E] = vars[D] + tmp[0];
	vars[D] = vars[C];
	vars[C] = vars[B];
	vars[B] = vars[A];
	vars[A] = tmp[0] + tmp[1];
}

void			sha256_rounds(uint32_t hash_value[8], unsigned char *message)
{
	int			i;
	uint32_t	message_schedule[64];
	uint32_t	working_variables[8];

	i = -1;
	while (++i < 8)
		working_variables[i] = hash_value[i];
	prepare_message_schedule(message, message_schedule);
	i = 0;
	while (i < 64)
		sha256_round(i++, message_schedule, working_variables);
	i = -1;
	while (++i < 8)
		hash_value[i] += working_variables[i];
}

unsigned char	*sha256_transform(char *pre_image)
{
	size_t			len;
	size_t			position;
	unsigned char	*padded_pre_image;
	uint32_t		hash_value[8];
	unsigned char	message[64];

	position = 0;
	sha256_hash_init(hash_value);
	padded_pre_image = sha256_preprocessing(pre_image, &len);
	while (position < FROM_BITS(len))
	{
		ft_memcpy((void*)message
				, (void*)(padded_pre_image + position)
				, sizeof(uint32_t) * 16);
		sha256_rounds(hash_value, message);
		position += FROM_BITS(512);
	}
	free(padded_pre_image);
	return (reverse_bytes(hash_value));
}

void			sha256(void *input)
{
	size_t			i;
	size_t			total;
	t_digest		*digest;
	t_hash_state	*state;
	char			output[65];

	i = 0;
	state = (t_hash_state*)input;
	total = state->digests->current;
	while (i < total)
	{
		digest = (t_digest*)&((char*)state->digests->buf)[i];
		if (digest->type != NO_INPUT)
			digest->hash_value = from_hex_hash(output
				, sha256_transform(digest->pre_image), 32);
		i += sizeof(t_digest);
	}
	print_hash_state("SHA256", state);
	free(state->digests->buf);
	free(state->digests);
	free(state);
}
