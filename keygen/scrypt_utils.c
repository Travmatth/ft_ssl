/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scrypt_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/15 21:36:34 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/18 13:16:05 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

void	verify_scrypt_params(unsigned parallel_param
	, unsigned block_size
	, unsigned cost_param)
{
	if (parallel_param < 1
		|| (block_size < 1 || block_size >= (1UL << (31 - 7)) / block_size)
		|| (cost_param < 2 || ((cost_param & (cost_param - 1)) != 0
			|| cost_param >= (1UL << (31 - 7)) / cost_param)))
		ft_ssl_err("error: incorrect scrypt configuration");
}

void	xor_word(uint32_t len, uint32_t *src, uint32_t *dst)
{
	len /= 8;
	while (len)
	{
		dst[0] ^= src[0];
		dst[1] ^= src[1];
		dst[2] ^= src[2];
		dst[3] ^= src[3];
		dst[4] ^= src[4];
		dst[5] ^= src[5];
		dst[6] ^= src[6];
		dst[7] ^= src[7];
		len -= 1;
		dst += 8;
		src += 8;
	}
}

void	to_little_endian(unsigned char *blocks
						, uint32_t *cost_tmp
						, uint32_t len)
{
	uint32_t	i;

	i = 0;
	while (i < len)
	{
		cost_tmp[i] = blocks[0]
					| (blocks[1] << 8)
					| (blocks[2] << 16)
					| (blocks[3] << 24);
		blocks += 4;
		i += 1;
	}
}

void	to_big_endian(unsigned char *blocks, uint32_t *cost_tmp, uint32_t len)
{
	uint32_t	i;
	uint32_t	desired;

	i = 0;
	while (i < len)
	{
		desired = cost_tmp[i];
		blocks[0] = desired;
		blocks[1] = desired >> 8;
		blocks[2] = desired >> 16;
		blocks[3] = desired >> 24;
		blocks += 4;
		i += 1;
	}
}
