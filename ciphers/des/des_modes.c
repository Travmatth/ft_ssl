/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_modes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/02 12:52:40 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/18 17:58:39 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

void		process_init_vector(t_desctx *ctx)
{
	uint8_t	*iv;

	iv = (uint8_t*)getpass("enter initialization vector (in hex)");
	if (!ft_htouint64(iv, &ctx->init_vector))
	{
		ft_putendl("error: inavlid initizaliation vector");
		process_init_vector(ctx);
	}
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
	if (GET_E(ctx->flags))
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
	(void)permuted_block;
	(void)iv;
	if (GET_E(ctx->flags))
	{
		ctx->init_vector = *permuted_block;
		return ;
	}
	*permuted_block ^= ctx->init_vector;
	ctx->init_vector = *block;
}

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
