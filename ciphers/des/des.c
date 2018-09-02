/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/13 11:01:21 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/02 15:41:30 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"
#include "../../includes/des_constants.h"

int			des_init(t_desctx *ctx, uint64_t keyschedule[16])
{
	uint8_t		i;
	uint64_t	key;
	uint32_t	left[16];
	uint32_t	right[16];
	uint8_t		keyshift[16];

	if (!ft_htouint64(ctx->key, &key))
		ft_ssl_err("error: invalid key");
	i = 0;
	left[0] = key;
	right[0] = key << 28;
	keyshift = GET_DECRYPT(ctx->flags) ? g_des_key_dec : g_des_key_enc;
	if (GET_DECRYPT(ctx->flags) && GET_A(ctx->flags))
	{
		ctx->plaintext = base64_decode((char*)ctx->plaintext, ctx->plen);
		ctx->plen = LEN(ctx->plaintext, 0);
	}
	while (i < 16)
	{
		left[i] = R_LEFT(left[i - 1], keyshift[i]);
		right[i] = R_LEFT(right[i - 1], keyshift[i]);
		key = ((uint64_t)left << sizeof(uint32_t)) | (uint64_t)right;
		keyschedule[i++] = permute_block(g_key_perm, key);
	}
	key_operation_mode(GET_DECRYPT(ctx->flags), keyschedule);
}


int			des_update(t_desctx *ctx
					, uint8_t *plaintext
					, uint64_t keyschedule[16])
{
	uint64_t	block;
	uint64_t	permuted_block;

	block = ft_uint8to64(ctx->plaintext);
	ctx->pre_permute_chaining(ctx, &block, plaintext, keyschedule);
	permuted_block = des_permute(ctx, block, plaintext, keyschedule);
	ctx->pre_permute_chaining(ctx, &permuted_block, plaintext, keyschedule);
	ft_uint64to8(permuted_block, ctx->ciphertext + ctx->clen);
	ctx->clen += 8;
	return (1);
}

int			des_final(t_desctx *ctx
					, uint8_t *plaintext
					, uint64_t keyschedule[16]
					, size_t plen)
{
	uint8_t	tmp[8];
	uint8_t	pad;
	ssize_t	rem;

	if ((rem = ctx->plen % 8) && GET_NP(ctx->flags))
		ft_ssl_err("error");
	else if (rem)
	{
		ft_memcpy((void*)tmp, plaintext, rem * sizeof(uint8_t));
		pad = (uint8_t)(8 - rem);
		ft_czero(tmp + rem * sizeof(uint8_t), pad, pad);
		des_update(ctx, tmp, keyschedule);
	}
}

void		des_wrapper_print(t_desctx *ctx)
{
	char	*out;
	
	if (GET_ENCRYPT(ctx->flags) && GET_A(ctx->flags))
	{
		ctx->ciphertext = base64_encode(ctx->ciphertext, ctx->clen);
		ctx->clen = ctx->clen / 3 * 4;
	}
	if (!(out = ft_strnew(ctx->clen)))
		ft_ssl_err("error");
	from_hex_hash(out, ctx->ciphertext, ctx->clen);
	// print ciphertext
	write(ctx->out_file, out, ctx->clen);
	free(ctx->plaintext);
	free(ctx);
}

void		des_wrapper(void *input)
{
	t_desctx	*ctx;
	uint8_t		*plaintext;
	uint64_t	keyschedule[16];
	size_t		clen;

	ctx  = (t_desctx*)input;
	clen = GET_A(ctx->flags) && GET_DECRYPT(ctx->flags)
		? ctx->plen / 3 * 4 : ctx->plen;
	if (!(ctx->ciphertext = ft_memalloc(clen)))
		ft_ssl_err("error");
	ctx->key = ctx->key ? ctx->key : create_des_key(ctx);
	plaintext = ctx->plaintext;
	des_init(ctx, keyschedule);
	while (ctx->plen >= 8)
	{
		des_update(ctx, plaintext, keyschedule);
		plaintext += 8;
		ctx->plen -= 8;
	}
	des_final(ctx, plaintext, keyschedule, ctx->plen);
	des_wrapper_print(ctx);
}