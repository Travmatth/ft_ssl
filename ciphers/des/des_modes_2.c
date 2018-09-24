/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_modes_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 20:18:43 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/23 18:39:24 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

/*
** In pcbc mode, the iv of next round is calculated by xor'ing the
** original plaintext with original plaintext and the new ciphertext
*/

void		des_pcbc_pre_permute_hook(t_desctx *ctx
									, uint64_t *block
									, uint64_t *permuted_block
									, uint64_t *iv)
{
	(void)permuted_block;
	if (GET_ENCRYPT(ctx->flags))
	{
		*iv = *block;
		*block ^= ctx->init_vector;
		return ;
	}
	*iv = *block;
}

void		des_pcbc_post_permute_hook(t_desctx *ctx
									, uint64_t *block
									, uint64_t *permuted_block
									, uint64_t *iv)
{
	(void)block;
	if (GET_ENCRYPT(ctx->flags))
	{
		ctx->init_vector = *iv ^ *permuted_block;
		return ;
	}
	*permuted_block ^= ctx->init_vector;
	ctx->init_vector = *iv ^ *permuted_block;
}

/*
** cfb mode is achieved by swapping the places of the iv and the text block
** and setting the next input block as the result of xor'ing the permuted block
** against the inputted text
*/

void		des_cfb_pre_permute_hook(t_desctx *ctx
									, uint64_t *block
									, uint64_t *permuted_block
									, uint64_t *iv)
{
	uint64_t	tmp;

	(void)permuted_block;
	if (GET_ENCRYPT(ctx->flags))
	{
		tmp = *block;
		*block = ctx->init_vector;
		*iv = *block;
		return ;
	}
	tmp = *block;
	*block = ctx->init_vector;
	*iv = *block;
}

void		des_cfb_post_permute_hook(t_desctx *ctx
									, uint64_t *block
									, uint64_t *permuted_block
									, uint64_t *iv)
{
	(void)block;
	if (GET_ENCRYPT(ctx->flags))
	{
		*permuted_block ^= *iv;
		ctx->init_vector = *permuted_block;
		return ;
	}
	*permuted_block ^= *iv;
	ctx->init_vector = *iv;
}
