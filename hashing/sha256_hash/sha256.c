/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 12:41:53 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/20 13:20:22 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

void	sha256_init(t_sha256 *ctx)
{
	ctx->len = 0;
	ctx->state[0] = 0x6a09e667;
	ctx->state[1] = 0xbb67ae85;
	ctx->state[2] = 0x3c6ef372;
	ctx->state[3] = 0xa54ff53a;
	ctx->state[4] = 0x510e527f;
	ctx->state[5] = 0x9b05688c;
	ctx->state[6] = 0x1f83d9ab;
	ctx->state[7] = 0x5be0cd19;
}

void	sha256_update(t_sha256 *ctx, uint8_t *message, size_t len)
{
	uint8_t	*ptr;
	size_t	rem;

	ptr = message;
	rem = ctx->len % SHA256_BLOCK_LEN;
	ctx->len += len;
	if (rem)
	{
		if (len < SHA256_BLOCK_LEN - rem)
		{
			ft_memcpy(ctx->buf + rem, ptr, len);
			return ;
		}
		ft_memcpy(ctx->buf + rem, ptr, SHA256_BLOCK_LEN - rem);
		len -= SHA256_BLOCK_LEN - rem;
		ptr += SHA256_BLOCK_LEN - rem;
		sha256_rounds(ctx->state, ctx->buf);
	}
	while (len >= SHA256_BLOCK_LEN)
	{
		sha256_rounds(ctx->state, ptr);
		len -= SHA256_BLOCK_LEN;
		ptr += SHA256_BLOCK_LEN;
	}
	ft_memcpy(ctx->buf, ptr, len);
}

void	sha256_final(t_sha256 *ctx, uint8_t *digest)
{
	int		i;

	sha256_pad_plaintext(ctx);
	i = -1;
	while (++i < SHA256_DIGEST_INT)
	{
		digest[4 * i] = ctx->state[i] >> 24;
		digest[4 * i + 1] = ctx->state[i] >> 16;
		digest[4 * i + 2] = ctx->state[i] >> 8;
		digest[4 * i + 3] = ctx->state[i];
	}
	sha256_init(ctx);
}

char	*sha256_full(uint8_t *in, size_t len)
{
	t_sha256	ctx;
	uint8_t		digest[SHA256_BLOCK_LEN];

	sha256_init(&ctx);
	sha256_update(&ctx, in, len);
	sha256_final(&ctx, digest);
	return (ft_strdup((char*)digest));
}

void	sha256_ssl_wrapper(void *input)
{
	size_t			i;
	size_t			total;
	t_digest		*digest;
	t_hash_state	*state;
	char			out[65];

	i = 0;
	state = (t_hash_state*)input;
	total = state->digests->current;
	while (i < total)
	{
		digest = (t_digest*)&((char*)state->digests->buf)[i];
		if (digest->type != NO_INPUT)
			digest->hash_value = from_hex_hash(out, (unsigned char*)sha256_full(
				(uint8_t*)digest->pre_image
				, LEN(digest->pre_image, 0)), 32);
		i += sizeof(t_digest);
	}
	print_hash_state("SHA256", state);
	free(state->digests->buf);
	free(state->digests);
	free(state);
}
