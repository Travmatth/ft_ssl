/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_algo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/02 12:55:44 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/23 18:37:44 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

static uint8_t	g_init_perm[64] =
{
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};

static uint8_t	g_des_exp[64] =
{
	32, 1, 2, 3, 4, 5,
	4, 5, 6, 7, 8, 9,
	8, 9, 10, 11, 12, 13,
	12, 13, 14, 15, 16, 17,
	16, 17, 18, 19, 20, 21,
	20, 21, 22, 23, 24, 25,
	24, 25, 26, 27, 28, 29,
	28, 29, 30, 31, 32, 1,
	0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0
};

static uint8_t	g_des_sboxes[8][4][16] =
{
	{
		{14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7},
		{0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8},
		{4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0},
		{15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13}
	},
	{
		{15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10},
		{3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5},
		{0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15},
		{13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9}
	},
	{
		{10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8},
		{13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1},
		{13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7},
		{1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12}
	},
	{
		{7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15},
		{13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9},
		{10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4},
		{3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14}
	},
	{
		{2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9},
		{14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6},
		{4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14},
		{11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3}
	},
	{
		{12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11},
		{10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8},
		{9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6},
		{4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13}
	},
	{
		{4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1},
		{13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6},
		{1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2},
		{6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12}
	},
	{
		{13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7},
		{1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2},
		{7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8},
		{2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11}
	}
};

static uint8_t	g_des_pbox[64] =
{
	16, 7, 20, 21, 29, 12, 28, 17,
	1, 15, 23, 26, 5, 18, 31, 10,
	2, 8, 24, 14, 32, 27, 3, 9,
	19, 13, 30, 6, 22, 11, 4, 25
};

static uint8_t	g_des_final_perm[64] =
{
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41, 9, 49, 17, 57, 25
};

/*
** permute_block transforms a given 64 bit block according to the map specified
*/

uint64_t		permute_block(uint8_t *map, uint64_t block, size_t limit)
{
	uint64_t	permuted;
	uint64_t	bit;
	uint8_t		i;

	i = 0;
	permuted = 0;
	while (i < limit)
	{
		bit = ((block >> (64 - map[i])) & 1);
		permuted |= (bit << (64 - (i++ + 1)));
	}
	return (permuted);
}

/*
** The fiestel cipher operates on 32 bits at a time, undergoing four stages:
** 1: expansion: from 32 -> 48 bits
** 2: key mixing: xor'ing the in_text with given subkey
** 3: substitution: s-boxes replaces given bit patterns
** 4: permutation: p-boxes spreads info across next rounds s-boxes
** These steps provide the confusion and diffusion
** needed in a cryptographic cipher
** en.wikipedia.org/wiki/Data_Encryption_Standard#The_Feistel_(F)_function
*/

uint64_t		feistel_cipher(uint64_t block, uint64_t key)
{
	int			i;
	uint64_t	permuted;
	uint8_t		inner;
	uint8_t		outer;

	permuted = 0;
	block = permute_block(g_des_exp, block, 48);
	block ^= key;
	i = 0;
	while (i < 8)
	{
		outer = (((block >> (64 - (6 * i + 1))) & 1) << 1)
				| ((block >> (64 - (6 * i + 6)) & 1));
		inner = (block << (6 * i + 1)) >> 60;
		permuted |= (uint64_t)g_des_sboxes[i][outer][inner] << (60 - (4 * i));
		i += 1;
	}
	return (permute_block(g_des_pbox, permuted, 32));
}

/*
** des_permute performs the pre and post permutations neccessary,
** as well as the 16 rounds of processing needed to encipher or decipher
** a given block of plain or ciphertext. En/Deciphering performed by the feistel
** cipher as given above
*/

uint64_t		des_permute(uint64_t block
							, uint64_t keyschedule[16]
							, int encipher)
{
	int			i;
	uint8_t		next;
	uint64_t	left;
	uint64_t	right;

	i = 0;
	block = permute_block(g_init_perm, block, 64);
	while (i < 16)
	{
		left = block << 32;
		right = block >> 32;
		next = encipher ? i++ : 15 - i++;
		right ^= (feistel_cipher(left, keyschedule[next]) >> 32);
		block = left | right;
	}
	block = (block << 32) | (block >> 32);
	return (permute_block(g_des_final_perm, block, 64));
}

void			verify_des_params(t_desctx *ctx)
{
	if (!ctx->out_file)
		ctx->out_file = STDOUT;
	if (GET_NEED_V(ctx->flags) && !GET_V(ctx->flags))
		process_init_vector(ctx);
	if (!ctx->key)
		create_des_key(ctx);
	if (!GET_DECRYPT(ctx->flags) && !GET_ENCRYPT(ctx->flags))
		SET_ENCRYPT(ctx->flags);
	if (!GET_INPUT(ctx->flags))
	{
		ctx->in_text = (uint8_t*)ft_str_from_fd(STDIN);
		ctx->i_len = LEN((char*)ctx->in_text, 0);
	}
}
