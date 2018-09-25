/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hmac.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 19:20:35 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/24 18:31:42 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

static const size_t g_diff = SHA256_BLOCK_LEN - SHA256_DIGEST_LEN;

void	xor_hmac(t_hmac *ctx, uint32_t *key, uint32_t pad)
{
	uint32_t	i;

	i = 0;
	while (i < SHA256_BLOCK_LEN)
	{
		ctx->sha256.buf[i] = ctx->sha256.buf[i] ^ pad;
		i += 1;
	}
	sha256_init(&ctx->sha256);
	sha256_update(&ctx->sha256, ctx->sha256.buf, SHA256_BLOCK_LEN);
	ft_memcpy(key, ctx->sha256.state, SHA256_DIGEST_LEN);
}

void	compress_key(t_hmac *ctx, uint8_t *key, size_t len)
{
	sha256_init(&ctx->sha256);
	sha256_update(&ctx->sha256, key, len);
	sha256_final(&ctx->sha256, ctx->sha256.buf);
	ft_memset(ctx->sha256.buf + SHA256_DIGEST_LEN, 0, g_diff);
}

void	hmac_sha256_init(t_hmac *ctx, uint8_t *key, size_t len)
{
	int		i;

	if (len <= SHA256_BLOCK_LEN)
	{
		ft_memcpy(ctx->sha256.buf, key, len);
		ft_memset(ctx->sha256.buf + len, 0, SHA256_BLOCK_LEN - len);
	}
	else
		compress_key(ctx, key, len);
	i = -1;
	while (++i < SHA256_BLOCK_LEN)
		ctx->sha256.buf[i] = ctx->sha256.buf[i] ^ OUTER_PAD;
	sha256_init(&ctx->sha256);
	sha256_update(&ctx->sha256, ctx->sha256.buf, SHA256_BLOCK_LEN);
	// TEST SHIM
	ft_printf("sha256 initial state: %lu", ctx->sha256.state[0]);
	// TEST SHIM
	ft_memcpy(ctx->outer, ctx->sha256.state, SHA256_DIGEST_LEN);
	i = -1;
	while (++i < SHA256_BLOCK_LEN)
		ctx->sha256.buf[i] = (ctx->sha256.buf[i] ^ OUTER_PAD) ^ INNER_PAD;
	sha256_init(&ctx->sha256);
	sha256_update(&ctx->sha256, ctx->sha256.buf, SHA256_BLOCK_LEN);
	ft_memcpy(ctx->inner, ctx->sha256.state, SHA256_DIGEST_LEN);
}

void	hmac_sha256_update(t_hmac *ctx, uint8_t *in, size_t len)
{
	sha256_update(&ctx->sha256, in, len);
}

void	hmac_sha256_final(t_hmac *ctx, uint8_t *digest)
{
	sha256_final(&ctx->sha256, digest);
	ctx->sha256.len = SHA256_BLOCK_LEN;
	ft_memcpy(ctx->sha256.state, ctx->outer, SHA256_BLOCK_LEN);
	sha256_update(&ctx->sha256, digest, SHA256_DIGEST_LEN);
	sha256_final(&ctx->sha256, digest);
	ctx->sha256.len = SHA256_BLOCK_LEN;
	ft_memcpy(ctx->sha256.state, ctx->inner, SHA256_BLOCK_LEN);
}
