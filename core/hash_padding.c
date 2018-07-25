/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hash_padding.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 20:00:36 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/23 12:33:35 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ssl.h"

size_t		get_hash_padding(size_t len)
{
	size_t a;

	a = 0;
	while (a * 512 < (len + 64 + 1))
		a += 1;
	return (a * 512 - 64 - 1 - len);
}

/*
** pad pre-image in accordance with md5 spec
*/

unsigned char	*pad_pre_image(char *pre_image, size_t *len)
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
	out = ft_uint32_to_chr((unsigned char*)ft_strnew(FROM_BITS(*len))
		, (uint32_t*)padded_pre_image
		, FROM_BITS(*len));
	free(padded_pre_image);
	return (out);
}
