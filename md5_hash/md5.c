/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/30 20:13:01 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/20 15:01:19 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ssl.h"

void	to_bin(t_fmt *fmt)
{
	fmt->out = ft_uintmaxtoa_base(*(size_t*)fmt->type.void_ptr, 2, "01");
	fmt->len.out = LEN(fmt->out, 0);
	ftprintf_format_nbr(fmt);
}

static size_t	get_md5_padding(size_t len)
{
	size_t a;

	a = 0;
	while (a * 512 < (len + 64 + 1))
		a += 1;
	return (a * 512 - 64 - 1 - len);
}

/*
** ft_uint32_to_chrs input (UINT4) into output (unsigned char).
** Assumes len is a multiple of 4.
*/

static unsigned char *ft_uint32_to_chr(unsigned char *output
	, uint32_t *input
	, unsigned int len)
{
	unsigned int i;
	unsigned int j;

	i = 0;
	j = 0;
	while (j < len)
	{
		output[j] = (unsigned char)(input[i] & 0xff);
		output[j + 1] = (unsigned char)((input[i] >> 8) & 0xff);
		output[j + 2] = (unsigned char)((input[i] >> 16) & 0xff);
		output[j + 3] = (unsigned char)((input[i] >> 24) & 0xff);
		i += 1;
		j += 4;
	}
	return (output);
}

/*
** Convert unsigned string to uint32_t array.
** Assumes len is a multiple of 4.
*/

static uint32_t	*ft_chr_to_uint32(uint32_t *output
	, unsigned char *input
	, unsigned int len)
{
	unsigned int i;
	unsigned int j;

	i = 0;
	j = 0;
	while (j < len)
	{
		output[i] = ((uint32_t)input[j])
			| (((uint32_t)input[j + 1]) << 8)
			| (((uint32_t)input[j + 2]) << 16)
			| (((uint32_t)input[j + 3]) << 24);
		i += 1;
		j += 4;
	}
	return (output);
}

unsigned char	*pad_pre_image(char *pre_image, size_t *len)
{
	size_t			orig_bit_len;
	size_t			padding_bit_len;
	unsigned char	*padded_pre_image;
	unsigned char	*out;

	orig_bit_len = TO_BITS(LEN(pre_image, 0));
	padding_bit_len = get_md5_padding(orig_bit_len);
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

unsigned char	*md5_transform(char *pre_image)
{
	size_t		len;
	size_t		position;
	unsigned char		*padded_pre_image;
	uint32_t	chaining_vars[4];
	uint32_t	message[16];
	uint32_t	a;
	uint32_t	b;
	uint32_t	c;
	uint32_t	d;

	position = 0;
	chaining_vars[A] = 0x67452301;
	chaining_vars[B] = 0xefcdab89;
	chaining_vars[C] = 0x98badcfe;
	chaining_vars[D] = 0x10325476;
	padded_pre_image = pad_pre_image(pre_image, &len);
	len = FROM_BITS(len);

	while (position < len)
	{
		ft_chr_to_uint32((uint32_t*)message
			, padded_pre_image + position
			, sizeof(uint32_t) * 16);
		a = chaining_vars[A];
		b = chaining_vars[B];
		c = chaining_vars[C];
		d = chaining_vars[D];

		FF(a, b, c, d, message[0], 7, 0xd76aa478);
		FF(d, a, b, c, message[1], 12, 0xe8c7b756);
		FF(c, d, a, b, message[2], 17, 0x242070db);
		FF(b, c, d, a, message[3], 22, 0xc1bdceee);
		FF(a, b, c, d, message[4], 7, 0xf57c0faf);
		FF(d, a, b, c, message[5], 12, 0x4787c62a);
		FF(c, d, a, b, message[6], 17, 0xa8304613);
		FF(b, c, d, a, message[7], 22, 0xfd469501);
		FF(a, b, c, d, message[8], 7, 0x698098d8);
		FF(d, a, b, c, message[9], 12, 0x8b44f7af);
		FF(c, d, a, b, message[10], 17, 0xffff5bb1);
		FF(b, c, d, a, message[11], 22, 0x895cd7be);
		FF(a, b, c, d, message[12], 7, 0x6b901122);
		FF(d, a, b, c, message[13], 12, 0xfd987193);
		FF(c, d, a, b, message[14], 17, 0xa679438e);
		FF(b, c, d, a, message[15], 22, 0x49b40821);

		GG(a, b, c, d, message[1], 5, 0xf61e2562);
		GG(d, a, b, c, message[6], 9, 0xc040b340);
		GG(c, d, a, b, message[11], 14, 0x265e5a51);
		GG(b, c, d, a, message[0], 20, 0xe9b6c7aa);
		GG(a, b, c, d, message[5], 5, 0xd62f105d);
		GG(d, a, b, c, message[10], 9, 0x02441453);
		GG(c, d, a, b, message[15], 14, 0xd8a1e681);
		GG(b, c, d, a, message[4], 20, 0xe7d3fbc8);
		GG(a, b, c, d, message[9], 5, 0x21e1cde6);
		GG(d, a, b, c, message[14], 9, 0xc33707d6);
		GG(c, d, a, b, message[3], 14, 0xf4d50d87);
		GG(b, c, d, a, message[8], 20, 0x455a14ed);
		GG(a, b, c, d, message[13], 5, 0xa9e3e905);
		GG(d, a, b, c, message[2], 9, 0xfcefa3f8);
		GG(c, d, a, b, message[7], 14, 0x676f02d9);
		GG(b, c, d, a, message[12], 20, 0x8d2a4c8a);

		HH(a, b, c, d, message[5], 4, 0xfffa3942);
		HH(d, a, b, c, message[8], 11, 0x8771f681);
		HH(c, d, a, b, message[11], 16, 0x6d9d6122);
		HH(b, c, d, a, message[14], 23, 0xfde5380c);
		HH(a, b, c, d, message[1], 4, 0xa4beea44);
		HH(d, a, b, c, message[4], 11, 0x4bdecfa9);
		HH(c, d, a, b, message[7], 16, 0xf6bb4b60);
		HH(b, c, d, a, message[10], 23, 0xbebfbc70);
		HH(a, b, c, d, message[13], 4, 0x289b7ec6);
		HH(d, a, b, c, message[0], 11, 0xeaa127fa);
		HH(c, d, a, b, message[3], 16, 0xd4ef3085);
		HH(b, c, d, a, message[6], 23, 0x04881d05);
		HH(a, b, c, d, message[9], 4, 0xd9d4d039);
		HH(d, a, b, c, message[12], 11, 0xe6db99e5);
		HH(c, d, a, b, message[15], 16, 0x1fa27cf8);
		HH(b, c, d, a, message[2], 23, 0xc4ac5665);

		II(a, b, c, d, message[0], 6, 0xf4292244);
		II(d, a, b, c, message[7], 10, 0x432aff97);
		II(c, d, a, b, message[14], 15, 0xab9423a7);
		II(b, c, d, a, message[5], 21, 0xfc93a039);
		II(a, b, c, d, message[12], 6, 0x655b59c3);
		II(d, a, b, c, message[3], 10, 0x8f0ccc92);
		II(c, d, a, b, message[10], 15, 0xffeff47d);
		II(b, c, d, a, message[1], 21, 0x85845dd1);
		II(a, b, c, d, message[8], 6, 0x6fa87e4f);
		II(d, a, b, c, message[15], 10, 0xfe2ce6e0);
		II(c, d, a, b, message[6], 15, 0xa3014314);
		II(b, c, d, a, message[13], 21, 0x4e0811a1);
		II(a, b, c, d, message[4], 6, 0xf7537e82);
		II(d, a, b, c, message[11], 10, 0xbd3af235);
		II(c, d, a, b, message[2], 15, 0x2ad7d2bb);
		II(b, c, d, a, message[9], 21, 0xeb86d391);

		chaining_vars[A] += a;
		chaining_vars[B] += b;
		chaining_vars[C] += c;
		chaining_vars[D] += d;
		ft_uint32_to_chr((unsigned char*)message
			, (uint32_t*)padded_pre_image + position
			, sizeof(uint32_t) * 16);
		position += FROM_BITS(512);
	}
	free(padded_pre_image);
	return ft_uint32_to_chr((unsigned char*)ft_strnew(MD5_HASH_SIZE)
		, (uint32_t*)chaining_vars
		, sizeof(uint32_t) * 4);
}

char	*from_hex_hash(char *output, unsigned char *hash_value)
{
	size_t	i;

	i = 0;
	while (i < 16)
	{
		ft_snprintf(output + (i * 2), 3, "%02x\n", hash_value[i]);
		i += 1;
	}
	output[32] = '\0';
	free(hash_value);
	return (ft_strdup(output));
}

void	md5(void *input)
{
	size_t		i;
	size_t		total;
	t_digest	*digest;
	t_md5_state	*state;
	char		output[33];

	i = 0;
	state = (t_md5_state*)input;
	total = state->digests->current;
	while (i < total)
	{
		digest = (t_digest*)&((char*)state->digests->buf)[i];
		if (digest->type != NO_INPUT)
			digest->hash_value = from_hex_hash(output
				, md5_transform(digest->pre_image));
		i += sizeof(t_digest);
	}
	print_md5_state(state);
	free(state->digests->buf);
	free(state->digests);
	free(state);
}
