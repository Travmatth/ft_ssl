/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/13 11:01:21 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/08 12:46:39 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"
#include "../../includes/des_constants.h"

void			des_init(t_desctx *ctx, uint64_t keyschedule[16])
{
	int			i;
	uint64_t	key;
	uint64_t	left[16];
	uint64_t	right[16];
	int			*keyshift;

	if (!ft_htouint64(ctx->key, &key))
		ft_ssl_err("error: invalid key");
	// key = des_htob((char*)ctx->key);
	i = 0;
	key = permute_block(g_des_init_key_perm, key, 56);
	left[0] = (key >> 36) << 36;
	right[0] = key << 28;
	// keyschedule[0] = permute_block(g_key_perm, left[0] | (right[0] >> 28), 48);
	keyshift = GET_DECRYPT(ctx->flags) ? g_des_key_dec : g_des_key_enc;
	while (i < 16)
	{
		left[i] = (((left[i ? i - 1 : 0] | (left[i ? i - 1 : 0] >> 28)) << keyshift[i]) >> 36) << 36;
		right[i] = (((right[i ? i - 1 : 0] | (right[i ? i - 1 : 0] >> 28)) << keyshift[i]) >> 36) << 36;
		keyschedule[i] = permute_block(g_key_perm, left[i] | (right[i] >> 28), 48);
		i += 1;
	}
	key_operation_mode(GET_DECRYPT(ctx->flags), keyschedule);
}

void			des_update(t_desctx *ctx
					, uint8_t *plaintext
					, uint64_t keyschedule[16])
{
	uint64_t	block;
	uint64_t	permuted_block;

	block = ft_uint8to64(plaintext);
	ctx->pre_permute_chaining(ctx, &block, plaintext, keyschedule);
	permuted_block = des_permute(block, keyschedule);
	ctx->post_permute_chaining(ctx, &permuted_block, plaintext, keyschedule);
	ft_uint64to8(permuted_block, ctx->ciphertext + ctx->clen);
	ctx->clen += 8;
}

void			des_final(t_desctx *ctx
					, uint8_t *plaintext
					, uint64_t keyschedule[16]
					, size_t plen)
{
	uint8_t	tmp[8];
	uint8_t	pad;
	ssize_t	rem;

	rem = plen % 8;
	if (rem)
	{
		ft_memcpy((void*)tmp, plaintext, rem * sizeof(uint8_t));
		pad = (uint8_t)(8 - rem);
		ft_czero(tmp + rem * sizeof(uint8_t), pad, pad);
	}
	else
		ft_czero(tmp, 8, 8);
	des_update(ctx, tmp, keyschedule);
}

void		des_wrapper_print(t_desctx *ctx)
{
	char	*out;

	if (!(out = ft_strnew(ctx->clen)))
		ft_ssl_err("error");
	if (GET_ENCRYPT(ctx->flags) && GET_A(ctx->flags))
	{
		out = base64_encode((char*)ctx->ciphertext, ctx->clen);
		ctx->clen = ctx->clen / 3 * 4;
	}
	write(ctx->out_file, out, ctx->clen);
	free(ctx->plaintext);
	free(ctx);
}

void		configure_des_params(t_desctx *ctx)
{
	size_t		clen;

	clen = ctx->plen + 8;
	if (!(ctx->ciphertext = (uint8_t*)ft_strnew(clen)))
		ft_ssl_err("error");
	if (!ctx->key)
	{
		ctx->klen = 8;
		ctx->key = create_des_key(ctx);
	}
	if (GET_DECRYPT(ctx->flags) && GET_A(ctx->flags))
	{
		ctx->plaintext = (uint8_t*)base64_decode((char*)ctx->plaintext, ctx->plen);
		ctx->plen = LEN((char*)ctx->plaintext, 0);
	}
}

void		des_wrapper(void *input)
{
	t_desctx	*ctx;
	uint8_t		*plaintext;
	uint64_t	keyschedule[16];

	ctx = (t_desctx*)input;
	configure_des_params(ctx);
	des_init(ctx, keyschedule);
	plaintext = ctx->plaintext;
	while (ctx->plen >= 8)
	{
		des_update(ctx, plaintext, keyschedule);
		plaintext += 8;
		ctx->plen -= 8;
	}
	des_final(ctx, plaintext, keyschedule, ctx->plen);
	des_wrapper_print(ctx);
}
