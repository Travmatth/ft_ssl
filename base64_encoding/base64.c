/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/08 21:12:36 by tmatthew          #+#    #+#             */
/*   Updated: 2018/08/13 11:02:14 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

static const unsigned char decode[] = {
    66,66,66,66,66,66,66,66,66,66,64,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,63,52,53,
    54,55,56,57,58,59,60,61,66,66,66,65,66,66,66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,66,66,66,66,66,26,27,28,
    29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66
};

char	*base64_decode(char *message, size_t len)
{
	unsigned char	c;
	char			iter;
	char			*last;
	uint32_t		buf;
	char			*out;
	char			*result;

	buf = 0;
	iter = 0;
	last = message + len;
	out = ft_strnew((len / 3) * 4);
	result = out;
	while (message < last)
	{
		c = decode[(unsigned)*message++];
		if (c == ' ')
			continue ;
		else if (c == 66)
			return (NULL);
		else if (c == 65)
		{
			message = last;
			continue ;
		}
		else
		{
			buf = buf << 6 | c;
			iter += 1;
			if (iter == 4)
			{
				*(out++) = (buf >> 16) & 255;
				*(out++) = (buf >> 8) & 255;
				*(out++) = buf & 255;
				buf = 0;
				iter = 0;
			}
		}
	}
	if (iter == 3)
	{
		*(out++) = (buf >> 10) & 255;
		*(out++) = (buf >> 2) & 255;
	}
	else if (iter == 2)
	{
		*(out++) = (buf >> 4) & 255;
	}
	return (result);
}

size_t	base64_spread_chars(char *data, size_t x, size_t len, char *result, size_t out_i)
{
	uint32_t	n;
	char		*lookup;
	uint8_t		numbers[4];

	lookup = CHRS;
	n = ((uint32_t)data[x]) << 16;
	if (x + 1 < len)
		n += ((uint32_t)data[x + 1]) << 8;
	if ((x + 2) < len)
		n += data[x + 2];
	numbers[0] = (uint8_t)(n >> 18) & 63;
	numbers[1] = (uint8_t)(n >> 12) & 63;
	numbers[2] = (uint8_t)(n >> 6) & 63;
	numbers[3] = (uint8_t)n & 63;
	result[out_i++] = lookup[numbers[0]];
	result[out_i++] = lookup[numbers[1]];
	if ((x + 1) < len)
		result[out_i++] = lookup[numbers[2]];
	if ((x + 2) < len)
		result[out_i++] = lookup[numbers[3]];
	return (out_i);
}

char	*base64_encode(char *message, size_t base_len)
{
	uint8_t		*data;
	size_t		out_i;
	size_t		x;
	size_t		out_len;
	char		*result;

	out_i = 0;
	data = (uint8_t*)message;
	x = 0;
	out_len = (base_len * 4) / 3;
	while ((out_len % 4) != 0)
		out_len += 1;
	result = ft_strnew(out_len);
	while (x < base_len)
	{
		out_i = base64_spread_chars(message, x, base_len, result, out_i);
		x += 3;
	}
	if (base_len % 3)
	{
		x = 0;
		while (x < (base_len % 3) - 1)
		{
			result[out_i++] = '=';
			x += 1;
		}
	}
	return (result);
}

void	base64(void *input)
{
	t_base64	*state;
	size_t		base_len;
	char		*result;

	state = (t_base64*)input;
	base_len = LEN(state->message, 0);
	if (GET_D(state->mode))
		result = base64_decode(state->message, base_len);
	else
		result = base64_encode(state->message, base_len);
}
