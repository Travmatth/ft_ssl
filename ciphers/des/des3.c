/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des3.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 19:35:30 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/25 21:34:22 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

/*
** des3_init processes des params to initialize the cipher's state
** generates 16 element key schedule from given uint8_t* key
*/

void		des3_init(t_desctx *ctx, uint64_t **keyschedule, uint8_t *hex_key)
{
	size_t	i;
	uint8_t	*subkeys[3];
	int		encipher;

	if (!(keyschedule = (uint64_t**)ft_memalloc(3 * sizeof(uint64_t*))))
		ft_ssl_err("error");
	i = 0;
	encipher = GET_ENCRYPT(ctx->flags);
	subkeys[0] = encipher ? hex_key : hex_key + 32;
	subkeys[1] = hex_key + 16;
	subkeys[2] = encipher ? hex_key + 32 : hex_key;
	while (i < 3)
	{
		if (!(keyschedule[i] = (uint64_t*)ft_memalloc(16 * sizeof(uint64_t))))
			ft_ssl_err("error");
		des_init(keyschedule[i], subkeys[i]);
		i += 1;
	}
}

/*
** des3_update performs one round of the des cipher on the given text
** uses callback functions on des state struct to perform cipher-specific
** chaining of the out_text blocks using the enciphered blocks
** and initilization vectors
*/

void		des3_update(t_desctx *ctx
					, uint8_t *in_text
					, uint64_t **keyschedule)
{
	uint64_t	block;
	uint64_t	permuted_block;
	uint64_t	iv;
	int			encipher;

	encipher = GET_ENCRYPT(ctx->flags);
	block = ft_uint8to64(in_text);
	ctx->pre_permute_chaining(ctx, &block, &permuted_block, &iv);
	permuted_block = des_permute(block, keyschedule[0], encipher);
	block = des_permute(block, keyschedule[1], !encipher);
	permuted_block = des_permute(block, keyschedule[2], encipher);
	ctx->post_permute_chaining(ctx, &block, &permuted_block, &iv);
	ft_uint64to8(permuted_block, ctx->out_text + ctx->o_len);
	ctx->o_len += 8;
}

/*
** des3_final finishes the des algorithm by
** alternatively padding the final block of in_text when encrypting
** or removing the padding of the in_text when decrypting
*/

void		des3_final(t_desctx *ctx
					, uint8_t *in_text
					, uint64_t *keyschedule[3]
					, size_t i_len)
{
	uint8_t	tmp[8];
	uint8_t	pad;
	ssize_t	rem;

	if (GET_ENCRYPT(ctx->flags))
	{
		if ((rem = i_len % 8))
		{
			ft_memcpy((void*)tmp, in_text, rem * sizeof(uint8_t));
			pad = (uint8_t)(8 - rem);
			ft_czero(tmp + rem * sizeof(uint8_t), pad, pad);
		}
		else
			ft_czero(tmp, 8, 8);
		des3_update(ctx, tmp, keyschedule);
	}
	else
		des_decode_trim_padding(ctx);
}

void		free_des3(t_desctx *ctx, uint64_t *keyschedule[3])
{
	free(ctx->key);
	free(ctx->in_text);
	if (!GET_A(ctx->flags) && !GET_ENCRYPT(ctx->flags))
		free(ctx->out_text);
	if (ctx->password)
		free(ctx->password);
	if (ctx->salt)
		free(ctx->salt);
	free(keyschedule[0]);
	free(keyschedule[0]);
	free(keyschedule[0]);
	free(keyschedule);
}

/*
** des3_wrapper provides a opaque wrapper around the des3 cipher
** for the ft_ssl program; accepts a struct containing the parsed
** options and manages the ciphering and printing of the given text
*/

void		des3_wrapper(void *input)
{
	t_desctx	*ctx;
	uint8_t		*in_text;
	uint64_t	**keyschedule;

	ctx = (t_desctx*)input;
	configure_des_params(ctx, "des3");
	des3_init(ctx, keyschedule, ctx->key);
	in_text = ctx->in_text;
	while (ctx->i_len >= 8)
	{
		des3_update(ctx, in_text, keyschedule);
		in_text += 8;
		ctx->i_len -= 8;
	}
	des3_final(ctx, in_text, keyschedule, ctx->i_len);
	des_wrapper_print(ctx);
	free_des3(ctx, keyschedule);
}
