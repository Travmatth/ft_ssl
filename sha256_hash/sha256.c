/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 12:41:53 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/22 21:09:31 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ssl.h"

uint32_t g_sha256_constants[64] =
{
	0x428a2f98,
	0x71374491,
	0xb5c0fbcf,
	0xe9b5dba5,
	0x3956c25b,
	0x59f111f1,
	0x923f82a4,
	0xab1c5ed5,
	0xd807aa98,
	0x12835b01,
	0x243185be,
	0x550c7dc3,
	0x72be5d74,
	0x80deb1fe,
	0x9bdc06a7,
	0xc19bf174,
	0xe49b69c1,
	0xefbe4786,
	0x0fc19dc6,
	0x240ca1cc,
	0x2de92c6f,
	0x4a7484aa,
	0x5cb0a9dc,
	0x76f988da,
	0x983e5152,
	0xa831c66d,
	0xb00327c8,
	0xbf597fc7,
	0xc6e00bf3,
	0xd5a79147,
	0x06ca6351,
	0x14292967,
	0x27b70a85,
	0x2e1b2138,
	0x4d2c6dfc,
	0x53380d13,
	0x650a7354,
	0x766a0abb,
	0x81c2c92e,
	0x92722c85,
	0xa2bfe8a1,
	0xa81a664b,
	0xc24b8b70,
	0xc76c51a3,
	0xd192e819,
	0xd6990624,
	0xf40e3585,
	0x106aa070,
	0x19a4c116,
	0x1e376c08,
	0x2748774c,
	0x34b0bcb5,
	0x391c0cb3,
	0x4ed8aa4a,
	0x5b9cca4f,
	0x682e6ff3,
	0x748f82ee,
	0x78a5636f,
	0x84c87814,
	0x8cc70208,
	0x90befffa,
	0xa4506ceb,
	0xbef9a3f7,
	0xc67178f2
};

unsigned char	*sha256_preprocessing(char *pre_image, uint32_t *hash_value, size_t *len)
{
	hash_value[0] = 0x6a09e667;
	hash_value[1] = 0xbb67ae85;
	hash_value[2] = 0x3c6ef372;
	hash_value[3] = 0xa54ff53a;
	hash_value[4] = 0x510e527f;
	hash_value[5] = 0x9b05688c;
	hash_value[6] = 0x1f83d9ab;
	hash_value[7] = 0x5be0cd19;
	return (pad_pre_image(pre_image, len));
}

void			prepare_message_schedule(uint32_t *message, uint32_t *schedule)
{
	uint32_t	i;
	uint32_t	tmp[2];

	i = 16;
	ft_memcpy((void*)schedule, (void*)message, sizeof(uint32_t) * 16);
	while (i < 64)
	{
		tmp[0] = R_RIGHT(schedule[i - 2], 17)
				^ R_RIGHT(schedule[i - 2], 19)
				^ (schedule[i - 2] >> 10);
		tmp[1] = R_RIGHT(schedule[i - 15], 7)
				^ R_RIGHT(schedule[i - 15], 18)
				^ (schedule[i - 15] >> 3);
		schedule[i] = tmp[0] + schedule[i - 7] + tmp[1] + schedule[i - 16];
		i += 1;
	}
}

void 			sha256_round(int i, uint32_t *message_schedule, uint32_t *vars)
{
	uint32_t	tmp[2];

	tmp[0] = vars[H]
		+ (R_RIGHT(vars[E], 6) ^ R_RIGHT(vars[E], 11) ^ R_RIGHT(vars[E], 25))
		+ ((vars[E] & vars[F]) ^ (~(vars[E]) & vars[G]))
		+ g_sha256_constants[i]
		+ message_schedule[i];
	tmp[1] = (R_RIGHT(vars[A], 2) ^ R_RIGHT(vars[A], 13) ^ R_RIGHT(vars[A], 22))
		+ ((vars[A] & vars[B]) ^ (vars[A] & vars[C]) ^ (vars[B] & vars[C]));
	vars[H] = vars[G];
	vars[G] = vars[F];
	vars[F] = vars[E];
	vars[E] = vars[D] + tmp[0];
	vars[D] = vars[C];
	vars[C] = vars[B];
	vars[B] = vars[A];
	vars[A] = tmp[0] + tmp[1];
}

void			sha256_rounds(uint32_t hash_value[8], uint32_t message[16])
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
	uint32_t		message[16];

	position = 0;
	padded_pre_image = sha256_preprocessing(pre_image, hash_value, &len);
	len = FROM_BITS(len);
	while (position < len)
	{
		ft_chr_to_uint32((uint32_t*)message, padded_pre_image + position
			, sizeof(uint32_t) * 16);
		sha256_rounds(hash_value, message);
		ft_uint32_to_chr((unsigned char*)message,
			(uint32_t*)padded_pre_image + position, sizeof(uint32_t) * 16);
		position += FROM_BITS(512);
	}
	free(padded_pre_image);
	reverse_bytes(hash_value);
	return (ft_uint32_to_chr((unsigned char*)ft_strnew(161)
							, (uint32_t*)hash_value
							, sizeof(uint32_t) * 8));
}

void	sha256(void *input)
{
	size_t			i;
	size_t			total;
	t_digest		*digest;
	t_hash_state	*state;
	char			output[64];

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
