/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256_helpers.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/22 12:41:53 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/26 14:29:28 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ssl.h"
#include "sha256.h"

void			prepare_message_schedule(unsigned char *message
										, uint32_t *schedule)
{
	uint32_t	i;
	uint32_t	j;

	i = 0;
	j = 0;
	while (i < 16)
	{
		schedule[i] = (message[j] << 24) | (message[j + 1] << 16)
					| (message[j + 2] << 8) | (message[j + 3]);
		i += 1;
		j += 4;
	}
	while (i < 64)
	{
		schedule[i] = SIG1(schedule[i - 2])
					+ schedule[i - 7]
					+ SIG0(schedule[i - 15])
					+ schedule[i - 16];
		i += 1;
	}
}

unsigned char	*reverse_bytes(uint32_t *hash)
{
	uint32_t		i;
	unsigned char	*out;

	if (!(out = (unsigned char*)ft_strnew(32)))
		return (NULL);
	i = 0;
	while (i < 4)
	{
		out[i] = (hash[0] >> (24 - i * 8)) & 0x000000ff;
		out[i + 4] = (hash[1] >> (24 - i * 8)) & 0x000000ff;
		out[i + 8] = (hash[2] >> (24 - i * 8)) & 0x000000ff;
		out[i + 12] = (hash[3] >> (24 - i * 8)) & 0x000000ff;
		out[i + 16] = (hash[4] >> (24 - i * 8)) & 0x000000ff;
		out[i + 20] = (hash[5] >> (24 - i * 8)) & 0x000000ff;
		out[i + 24] = (hash[6] >> (24 - i * 8)) & 0x000000ff;
		out[i + 28] = (hash[7] >> (24 - i * 8)) & 0x000000ff;
		i += 1;
	}
	return (out);
}

void			sha256_hash_init(uint32_t *hash_value)
{
	hash_value[0] = 0x6a09e667;
	hash_value[1] = 0xbb67ae85;
	hash_value[2] = 0x3c6ef372;
	hash_value[3] = 0xa54ff53a;
	hash_value[4] = 0x510e527f;
	hash_value[5] = 0x9b05688c;
	hash_value[6] = 0x1f83d9ab;
	hash_value[7] = 0x5be0cd19;
}

void			append_len(unsigned char *out, size_t len, size_t orig_bit_len)
{
	out[len - 1] = (unsigned char)orig_bit_len;
	out[len - 2] = ((uint64_t)orig_bit_len) >> 8;
	out[len - 3] = ((uint64_t)orig_bit_len) >> 16;
	out[len - 4] = ((uint64_t)orig_bit_len) >> 24;
	out[len - 5] = ((uint64_t)orig_bit_len) >> 32;
	out[len - 6] = ((uint64_t)orig_bit_len) >> 40;
	out[len - 7] = ((uint64_t)orig_bit_len) >> 48;
	out[len - 8] = ((uint64_t)orig_bit_len) >> 56;
}

unsigned char	*sha256_preprocessing(char *pre_image, size_t *len)
{
	unsigned char	*out;
	size_t			orig_bit_len;
	size_t			padding_bit_len;
	unsigned char	*padded_pre_image;

	orig_bit_len = TO_BITS(LEN(pre_image, 0));
	padding_bit_len = get_hash_padding(orig_bit_len);
	*len = orig_bit_len + 1 + padding_bit_len + 64;
	if (!(padded_pre_image = (unsigned char*)ft_strnew(FROM_BITS(*len))))
		ft_ssl_err("error");
	ft_memcpy(padded_pre_image, pre_image, FROM_BITS(orig_bit_len));
	padded_pre_image[FROM_BITS(orig_bit_len)] = 0x80;
	ft_bzero(padded_pre_image + FROM_BITS(orig_bit_len) + 1
		, FROM_BITS(padding_bit_len));
	ft_memcpy((void*)(padded_pre_image + FROM_BITS(orig_bit_len) + 1
		+ FROM_BITS(padding_bit_len)), (void*)&orig_bit_len, sizeof(size_t));
	out = (unsigned char*)ft_strnew(FROM_BITS(*len));
	out = ft_uint32_to_chr(out, (uint32_t*)padded_pre_image, FROM_BITS(*len));
	append_len(out, FROM_BITS(*len), orig_bit_len);
	free(padded_pre_image);
	return (out);
}
