/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_modes_2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 20:18:43 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/21 20:38:24 by tmatthew         ###   ########.fr       */
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
	if (GET_E(ctx->flags))
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
	if (GET_E(ctx->flags))
	{
		ctx->init_vector = *iv ^ *permuted_block;
		return ;
	}
	*permuted_block ^= ctx->init_vector;
	ctx->init_vector = *iv ^ *permuted_block;
}
