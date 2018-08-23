/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   salsa.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 18:42:33 by tmatthew          #+#    #+#             */
/*   Updated: 2018/08/22 22:02:36 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

static void	salsa_rounds(uint32_t state[16], int rounds)
{
	// uint32_t	t;

	while (rounds)
	{
		state[4] ^= R_LEFT(state[0] + state[12], 7);
		state[8] ^= R_LEFT(state[4] + state[0], 9);
		state[12] ^= R_LEFT(state[8] + state[4], 13);
		state[0] ^= R_LEFT(state[12] + state[8], 18);
		state[9] ^= R_LEFT(state[5] + state[1], 7);
		state[13] ^= R_LEFT(state[9] + state[5], 9);
		state[1] ^= R_LEFT(state[13] + state[9], 13);
		state[5] ^= R_LEFT(state[1] + state[13], 18);
		state[14] ^= R_LEFT(state[10] + state[6], 7);
		state[2] ^= R_LEFT(state[14] + state[10], 9);
		state[6] ^= R_LEFT(state[2] + state[14], 13);
		state[10] ^= R_LEFT(state[6] + state[2], 18);
		state[3] ^= R_LEFT(state[15] + state[11], 7);
		state[7] ^= R_LEFT(state[3] + state[15], 9);
		state[11] ^= R_LEFT(state[7] + state[3], 13);
		state[15] ^= R_LEFT(state[11] + state[7], 18);
		state[1] ^= R_LEFT(state[0] + state[3], 7);
		state[2] ^= R_LEFT(state[1] + state[0], 9);
		state[3] ^= R_LEFT(state[2] + state[1], 13);
		state[0] ^= R_LEFT(state[3] + state[2], 18);
		state[6] ^= R_LEFT(state[5] + state[4], 7);
		state[7] ^= R_LEFT(state[6] + state[5], 9);
		state[4] ^= R_LEFT(state[7] + state[6], 13);
		state[5] ^= R_LEFT(state[4] + state[7], 18);
		state[11] ^= R_LEFT(state[10] + state[9], 7);
		state[8] ^= R_LEFT(state[11] + state[10], 9);
		state[9] ^= R_LEFT(state[8] + state[11], 13);
		state[10] ^= R_LEFT(state[9] + state[8], 18);
		state[12] ^= R_LEFT(state[15] + state[14], 7);
		state[13] ^= R_LEFT(state[12] + state[15], 9);
		state[14] ^= R_LEFT(state[13] + state[12], 13);
		state[15] ^= R_LEFT(state[14] + state[13], 18);
		rounds -= 2;
	}
}

void	salsa_core(uint32_t in[16], uint32_t out[16], int rounds)
{
	uint32_t tmp[16];

	tmp[0] = in[0];
	tmp[1] = in[1];
	tmp[2] = in[2];
	tmp[3] = in[3];
	tmp[4] = in[4];
	tmp[5] = in[5];
	tmp[6] = in[6];
	tmp[7] = in[7];
	tmp[8] = in[8];
	tmp[9] = in[9];
	tmp[10] = in[10];
	tmp[11] = in[11];
	tmp[12] = in[12];
	tmp[13] = in[13];
	tmp[14] = in[14];
	tmp[15] = in[15];
	salsa_rounds(tmp, rounds);
	out[0] = tmp[0] + in[0];
	out[1] = tmp[1] + in[1];
	out[2] = tmp[2] + in[2];
	out[3] = tmp[3] + in[3];
	out[4] = tmp[4] + in[4];
	out[5] = tmp[5] + in[5];
	out[6] = tmp[6] + in[6];
	out[7] = tmp[7] + in[7];
	out[8] = tmp[8] + in[8];
	out[9] = tmp[9] + in[9];
	out[10] = tmp[10] + in[10];
	out[11] = tmp[11] + in[11];
	out[12] = tmp[12] + in[12];
	out[13] = tmp[13] + in[13];
	out[14] = tmp[14] + in[14];
	out[15] = tmp[15] + in[15];
}
