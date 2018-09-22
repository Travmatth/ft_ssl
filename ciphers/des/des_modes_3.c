/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_modes_3.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/22 15:09:12 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/22 15:28:17 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

void		des_ofb_pre_permute_hook(t_desctx *ctx
									, uint64_t *block
									, uint64_t *permuted_block
									, uint64_t *iv)
{
	uint64_t	tmp;

	(void)permuted_block;
	if (GET_E(ctx->flags))
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

void		des_ofb_post_permute_hook(t_desctx *ctx
									, uint64_t *block
									, uint64_t *permuted_block
									, uint64_t *iv)
{
	(void)block;
	if (GET_E(ctx->flags))
	{
		ctx->init_vector = *permuted_block;
		*permuted_block ^= *iv;
		return ;
	}
	ctx->init_vector = *permuted_block;
	*permuted_block ^= *iv;
}

void		des_ctr_pre_permute_hook(t_desctx *ctx
									, uint64_t *block
									, uint64_t *permuted_block
									, uint64_t *iv)
{
	uint64_t	tmp;

	(void)permuted_block;
	if (GET_E(ctx->flags))
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

void		des_ctr_post_permute_hook(t_desctx *ctx
									, uint64_t *block
									, uint64_t *permuted_block
									, uint64_t *iv)
{
	(void)block;
	if (GET_E(ctx->flags))
	{
		*permuted_block ^= *iv;
		ctx->init_vector += 1;
		return ;
	}
	*permuted_block ^= *iv;
	ctx->init_vector += 1;
}
