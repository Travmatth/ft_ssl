/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/13 11:01:21 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/20 16:41:17 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

int		g_keyshift[16] =
{
	1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

uint8_t	g_des_init_key_perm[64] =
{
	57, 49, 41, 33, 25, 17, 9,
	1, 58, 50, 42, 34, 26, 18,
	10, 2, 59, 51, 43, 35, 27,
	19, 11, 3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14, 6, 61, 53, 45, 37, 29,
	21, 13, 5, 28, 20, 12, 4,
	0, 0, 0, 0, 0, 0, 0, 0
};

uint8_t g_key_perm[64] =
{
	14, 17, 11, 24, 1, 5,
	3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8,
	16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

/*
** des_init processes des params to initialize the cipher's state
** generates 16 element key schedule from given uint8_t* key
*/

void		des_init(t_desctx *ctx, uint64_t keyschedule[16])
{
	int			i;
	uint64_t	key;
	uint64_t	left[16];
	uint64_t	right[16];

	if (!ft_htouint64(ctx->key, &key))
		ft_ssl_err("error: invalid key");
	i = 0;
	key = permute_block(g_des_init_key_perm, key, 56);
	left[0] = (key >> 36) << 36;
	right[0] = key << 28;
	while (i < 16)
	{
		left[i] = ROT_28B_L(left[i ? i - 1 : 0], g_keyshift[i]);
		right[i] = ROT_28B_L(right[i ? i - 1 : 0], g_keyshift[i]);
		key = left[i] | (right[i] >> 28);
		keyschedule[i] = permute_block(g_key_perm, key, 48);
		i += 1;
	}
}

/*
** des_update performs one round of the des cipher on the given text
** uses callback functions on des state struct to perform cipher-specific
** chaining of the out_text blocks using the enciphered blocks
** and initilization vectors
*/

void		des_update(t_desctx *ctx
					, uint8_t *in_text
					, uint64_t keyschedule[16])
{
	uint64_t	block;
	uint64_t	permuted_block;
	uint64_t	iv;

	block = ft_uint8to64(in_text);
	ctx->pre_permute_chaining(ctx, &block, &permuted_block, &iv);
	permuted_block = des_permute(block, keyschedule, GET_ENCRYPT(ctx->flags));
	ctx->post_permute_chaining(ctx, &block, &permuted_block, &iv);
	ft_uint64to8(permuted_block, ctx->out_text + ctx->o_len);
	ctx->o_len += 8;
}

/*
** des_decode_trim_padding removes the padding of deciphered plaintexts
** des is padded in conformance with pkcs#7, meaning that N number of
** bytes (each of value N) are added onto the end of a given plaintext,
** where N >=1 & < <= 8
** ex: if len(plaintext) % 7 == 0, byte "\x01" is added as padding
*/

void		des_decode_trim_padding(t_desctx *ctx)
{
	uint8_t	pad;
	ssize_t	rem;

	if (ctx->o_len && ((pad = ctx->out_text[ctx->o_len - 1]) <= 8))
	{
		rem = (ssize_t)pad;
		while (pad)
			ctx->out_text[ctx->o_len - pad--] = '\0';
		ctx->o_len -= (size_t)rem;
	}
}

/*
** des_final finishes the des algorithm by
** alternatively padding the final block of in_text when encrypting
** or removing the padding of the in_text when decrypting
*/

void		des_final(t_desctx *ctx
					, uint8_t *in_text
					, uint64_t keyschedule[16]
					, size_t i_len)
{
	uint8_t	tmp[8];
	uint8_t	pad;
	ssize_t	rem;

	if (GET_E(ctx->flags))
	{
		if ((rem = i_len % 8))
		{
			ft_memcpy((void*)tmp, in_text, rem * sizeof(uint8_t));
			pad = (uint8_t)(8 - rem);
			ft_czero(tmp + rem * sizeof(uint8_t), pad, pad);
		}
		else
			ft_czero(tmp, 8, 8);
		des_update(ctx, tmp, keyschedule);
	}
	else
		des_decode_trim_padding(ctx);
}

/*
** des_wrapper provides a opaque wrapper around the des cipher
** for the ft_ssl program; accepts a struct containing the parsed
** options and manages the ciphering and printing of the given text
*/

void		des_wrapper(void *input)
{
	t_desctx	*ctx;
	uint8_t		*in_text;
	uint64_t	keyschedule[16];

	ctx = (t_desctx*)input;
	configure_des_params(ctx, "des");
	des_init(ctx, keyschedule);
	in_text = ctx->in_text;
	while (ctx->i_len >= 8)
	{
		des_update(ctx, in_text, keyschedule);
		in_text += 8;
		ctx->i_len -= 8;
	}
	des_final(ctx, in_text, keyschedule, ctx->i_len);
	des_wrapper_print(ctx);
	free(ctx->key);
	free(ctx->in_text);
	if (!GET_A(ctx->flags) && !GET_ENCRYPT(ctx->flags))
		free(ctx->out_text);
	if (ctx->password)
		free(ctx->password);
	if (ctx->salt)
		free(ctx->salt);
}
