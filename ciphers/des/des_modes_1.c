/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_modes_1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 20:11:43 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/16 19:03:54 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

/*
** In ecb mode, no xor'ing of the block or iv occurs
*/

void		des_null_permute_hook(t_desctx *ctx
									, uint64_t *block
									, uint64_t *permuted_block
									, uint64_t *iv)
{
	(void)ctx;
	(void)block;
	(void)permuted_block;
	(void)iv;
}

/*
** In cbc mode, we xor the translated 64bit block with the iv
*/

void		des_cbc_pre_permute_hook(t_desctx *ctx
									, uint64_t *block
									, uint64_t *permuted_block
									, uint64_t *iv)
{
	(void)permuted_block;
	(void)iv;
	if (GET_ENCRYPT(ctx->flags))
	{
		*block ^= ctx->init_vector;
		return ;
	}
	*iv = *block;
}

/*
** In cbc mode, we xor the permuted_block with the iv
** and set the iv to the newly created block
*/

void		des_cbc_post_permute_hook(t_desctx *ctx
									, uint64_t *block
									, uint64_t *permuted_block
									, uint64_t *iv)
{
	(void)block;
	if (GET_ENCRYPT(ctx->flags))
	{
		ctx->init_vector = *permuted_block;
		return ;
	}
	*permuted_block ^= ctx->init_vector;
	ctx->init_vector = *iv;
}
