/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/30 20:13:01 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/21 16:26:05 by tmatthew         ###   ########.fr       */
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

void			md5_round(short i, uint32_t message[], uint32_t vars[])
{
	j = 0;
	while (j < 16)
	{
		n = t_md5_rounds[i][j];
		if (!(j % 4))
			vars[0] = vars[1] + R_LEFT((vars[0] + n.f(vars[1], vars[2], vars[3])
						+ message[n.sub_block] + n.signed_constant), n.step));
		else if (j % 4 == 1)
			vars[3] = vars[0] + R_LEFT((vars[3] + n.f(vars[0], vars[1], vars[2])
						+ message[n.sub_block] + n.signed_constant), n.step));
		else if (j % 4 == 2)
			vars[2] = vars[3] + R_LEFT((vars[2] + n.f(vars[3], vars[0], vars[1])
						+ message[n.sub_block] + n.signed_constant), n.step));
		else
			vars[1] = vars[2] + R_LEFT((vars[1] + n.f(vars[2], vars[3], vars[0])
						+ message[n.sub_block] + n.signed_constant), n.step));
		j += 1;
	}
	return (vars);
}

void			md5_rounds(uint32_t message[], uint32_t chaining_vars[])
{
	short		i;
	uint32_t	vars[4];
	
	i = 0;
	while (i < 4)
	{
		vars[0] = chaining_vars[A];
		vars[1] = chaining_vars[B];
		vars[2] = chaining_vars[C];
		vars[3] = chaining_vars[D];
		md5_round(i, message, vars);
		chaining_vars[A] += a;
		chaining_vars[B] += b;
		chaining_vars[C] += c;
		chaining_vars[D] += d;
		i += 1;
	}
}

unsigned char	*md5_transform(char *pre_image)
{
	size_t		len;
	size_t		position;
	unsigned char		*padded_pre_image;
 	uint32_t	chaining_vars[4];
	uint32_t	message[16];
	short		i;
	short		j;
	t_md5_round	n;

	position = 0;
	chaining_vars[A] = 0x67452301;
	chaining_vars[B] = 0xefcdab89;
	chaining_vars[C] = 0x98badcfe;
	chaining_vars[D] = 0x10325476;
	padded_pre_image = pad_pre_image(pre_image, &len);
	len = FROM_BITS(len);
 	while (position < len)
	{
		ft_chr_to_uint32((uint32_t*)message, padded_pre_image + position, sizeof(uint32_t) * 16);
		md5_rounds(chaining_vars, message);
		ft_uint32_to_chr((unsigned char*)message, (uint32_t*)padded_pre_image + position, sizeof(uint32_t) * 16);
		position += FROM_BITS(512);
	}
	free(padded_pre_image);
	return ft_uint32_to_chr((unsigned char*)ft_strnew(MD5_HASH_SIZE), (uint32_t*)chaining_vars, sizeof(uint32_t) * 4);
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
