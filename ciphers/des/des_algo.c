/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_algo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/02 12:55:44 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/02 15:03:35 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

uint64_t	permute_block(uint8_t *map, uint64_t block)
{
	uint64_t	permuted;
	uint64_t	bit;
	uint8_t		i;

	i = 0;
	permuted = 0;
	while (i < 64)
	{
		bit = ((block >> (64 - map[i])) & 1);
		permuted |= (bit << (64 - (i++ + 1)));
	}
	return (permuted);
}

uint64_t	des_f(uint64_t	block, uint64_t key)
{
	int		i;
	uint64_t	permuted;
	uint8_t		inner;
	uint8_t		outer;

	permuted = 0;
	block = permute_block(g_des_exp, block);
	block ^= key;
	i = 0;
	while (i < 8)
	{
		outer = (((block >> (64 - (6 * i + 1))) & 1) << 1) | ((block >> (64 - (6 * i + 6)) & 1));
		inner = (block << (6 * i + 1)) >> 60;
		permuted |= (uint64_t)g_des_sboxes[i][outer][inner] << (60 - (4 * i));
		i += 1;
	}
	return (permute_block(g_des_pbox, permuted));
}

void		des_permute(t_desctx *ctx, uint64_t block, uint64_t keyschedule[16])
{
	uint8_t		i;
	uint64_t	left;
	uint64_t	right;

	i = 0;
	block = permute_block(g_init_perm, block);
	while (i < 16)
	{
		left = block << 32; 
		right = block >> 32; 
		right ^= (des_f(left, keyschedule) >> 32);
		block = left | right;
	}
	block = (block << 32) | (block >> 32);
	return (permute_block(g_des_final_perm, block));
}

void		key_operation_mode(int decrypt, uint64_t keyschedule[16])
{
	int			i;
	uint64_t	tmp;

	if (!decrypt)
		return ;
	i = 0;
	while (i < 16)
	{
		tmp = keyschedule[i];
		keyschedule[i] = keyschedule[15 - i];
		keyschedule[15 - i] = tmp;
		i += 1;
	}
}
