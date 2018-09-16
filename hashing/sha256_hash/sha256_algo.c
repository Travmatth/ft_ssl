/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256_algo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 12:41:53 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/15 15:33:25 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"
#include "../../includes/sha256.h"

static const uint32_t g_k[64] =
{
	0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5,
	0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
	0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3,
	0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
	0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc,
	0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
	0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7,
	0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
	0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13,
	0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
	0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3,
	0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
	0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5,
	0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
	0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208,
	0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2
};

void	prepare_message_schedule(unsigned char *message, uint32_t *schedule)
{
	uint32_t	i;
	uint32_t	j;

	i = 0;
	j = 0;
	while (i < 16)
	{
		schedule[i] = (message[j] << 24) | (message[j + 1] << 16)
					| (message[j + 2] << 8) | (message[j + 3]);
		i += 1;
		j += 4;
	}
	while (i < 64)
	{
		schedule[i] = SIG1(schedule[i - 2])
					+ schedule[i - 7]
					+ SIG0(schedule[i - 15])
					+ schedule[i - 16];
		i += 1;
	}
}

void	sha256_pad_plaintext(t_sha256 *ctx)
{
	size_t	rem;

	rem = ctx->len % SHA256_BLOCK_LEN;
	ctx->buf[rem++] = 0x80;
	if (rem > 56)
	{
		ft_memset((void*)(ctx->buf + rem), 0, SHA256_BLOCK_LEN - rem);
		rem = 0;
		sha256_rounds(ctx->state, ctx->buf);
	}
	ft_memset(ctx->buf + rem, 0, 56 - rem);
	ctx->len *= 8;
	ctx->buf[63] = (unsigned char)ctx->len;
	ctx->buf[62] = ((uint64_t)ctx->len) >> 8;
	ctx->buf[61] = ((uint64_t)ctx->len) >> 16;
	ctx->buf[60] = ((uint64_t)ctx->len) >> 24;
	ctx->buf[59] = ((uint64_t)ctx->len) >> 32;
	ctx->buf[58] = ((uint64_t)ctx->len) >> 40;
	ctx->buf[57] = ((uint64_t)ctx->len) >> 48;
	ctx->buf[56] = ((uint64_t)ctx->len) >> 56;
	sha256_rounds(ctx->state, ctx->buf);
}

void	sha256_round(int i, uint32_t *message_schedule, uint32_t *vars)
{
	uint32_t	tmp[2];

	tmp[0] = vars[H]
		+ EP1(vars[E])
		+ CH(vars[E], vars[F], vars[G])
		+ g_k[i]
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

void	sha256_rounds(uint32_t hash_value[8], unsigned char *message)
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
