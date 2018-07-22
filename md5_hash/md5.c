/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/30 20:13:01 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/21 20:54:34 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "md5.h"
#include "md5_constants.h"

extern t_md5_round	g_md5_rounds[4][16];

void			md5_round(short i, uint32_t *message, uint32_t *vars)
{
	short		j;
	t_md5_round	n;

	j = 0;
	while (j < 16)
	{
		n = g_md5_rounds[i][j];
		if (!(j % 4))
			vars[A] = vars[B] + R_LEFT((vars[A] + n.f(vars[B], vars[C], vars[D])
						+ message[n.sub_block] + n.signed_constant), n.step);
		else if (j % 4 == 1)
			vars[D] = vars[A] + R_LEFT((vars[D] + n.f(vars[A], vars[B], vars[C])
						+ message[n.sub_block] + n.signed_constant), n.step);
		else if (j % 4 == 2)
			vars[C] = vars[D] + R_LEFT((vars[C] + n.f(vars[D], vars[A], vars[B])
						+ message[n.sub_block] + n.signed_constant), n.step);
		else
			vars[B] = vars[C] + R_LEFT((vars[B] + n.f(vars[C], vars[D], vars[A])
						+ message[n.sub_block] + n.signed_constant), n.step);
		j += 1;
	}
}

void			md5_rounds(uint32_t *message, uint32_t *chaining_vars)
{
	short		i;
	uint32_t	vars[4];

	i = 0;
	vars[A] = chaining_vars[A];
	vars[B] = chaining_vars[B];
	vars[C] = chaining_vars[C];
	vars[D] = chaining_vars[D];
	while (i < 4)
	{
		md5_round(i, message, vars);
		i += 1;
	}
	chaining_vars[A] += vars[A];
	chaining_vars[B] += vars[B];
	chaining_vars[C] += vars[C];
	chaining_vars[D] += vars[D];
}

unsigned char	*md5_transform(char *pre_image)
{
	size_t				len;
	size_t				position;
	unsigned char		*padded_pre_image;
	uint32_t			chaining_vars[4];
	uint32_t			message[16];

	position = 0;
	chaining_vars[A] = 0x67452301;
	chaining_vars[B] = 0xefcdab89;
	chaining_vars[C] = 0x98badcfe;
	chaining_vars[D] = 0x10325476;
	padded_pre_image = pad_pre_image(pre_image, &len);
	len = FROM_BITS(len);
	while (position < len)
	{
		ft_chr_to_uint32((uint32_t*)message, padded_pre_image + position
			, sizeof(uint32_t) * 16);
		md5_rounds(message, chaining_vars);
		ft_uint32_to_chr((unsigned char*)message,
			(uint32_t*)padded_pre_image + position, sizeof(uint32_t) * 16);
		position += FROM_BITS(512);
	}
	free(padded_pre_image);
	return (ft_uint32_to_chr((unsigned char*)ft_strnew(MD5_HASH_SIZE)
		, (uint32_t*)chaining_vars, sizeof(uint32_t) * 4));
}

void			md5(void *input)
{
	size_t		i;
	size_t		total;
	t_digest	*digest;
	t_hash_state	*state;
	char		output[33];

	i = 0;
	state = (t_hash_state*)input;
	total = state->digests->current;
	while (i < total)
	{
		digest = (t_digest*)&((char*)state->digests->buf)[i];
		if (digest->type != NO_INPUT)
			digest->hash_value = from_hex_hash(output
				, md5_transform(digest->pre_image));
		i += sizeof(t_digest);
	}
	print_hash_state("MD5", state);
	free(state->digests->buf);
	free(state->digests);
	free(state);
}
