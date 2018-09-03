/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_modes.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/02 12:52:40 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/03 09:29:35 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

/*
** In cbc mode, we xor the translated 64bit block with the iv
*/

void		des_cbc_pre_permute_hook(t_desctx *ctx
	, uint64_t *block
	, uint8_t *plaintext
	, uint64_t keyschedule[16])
{
	uint64_t	iv;

	(void)plaintext;
	(void)keyschedule;
	*block ^= ft_uint8to64(ctx->init_vector);
}

/*
** In cbc mode, we xor the permuted_block with the iv
** and set the iv to the newly created block
*/

void		des_cbc_post_permute_hook(t_desctx *ctx
	, uint64_t *block
	, uint8_t *plaintext
	, uint64_t keyschedule[16])
{
	(void)plaintext;
	(void)keyschedule;
	*block ^= ft_uint8to64(ctx->init_vector);
	ctx->init_vector = ft_uint64to8(*block); 
}

/*
** In ecb mode, no xor'ing of the block or iv occurs
*/
void		des_null_permute_hook(t_desctx *ctx
	, uint64_t *block
	, uint8_t *plaintext
	, uint64_t keyschedule[16])
{
	(void)ctx;
	(void)block;
	(void)plaintext;
	(void)keyschedule;
}
