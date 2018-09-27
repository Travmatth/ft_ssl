/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b64_algo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/09 20:21:40 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/26 15:50:21 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

/*
** b64_finalize adds a newline character every 64 chars
** and one at the end, if necessary
*/

void	*b64_finalize(unsigned char *in, size_t *len)
{
	size_t			o_len;
	size_t			c;
	size_t			newlines;
	unsigned char	*out;

	o_len = *len + (*len / 64) + (*len % 64 ? 1 : 0);
	newlines = *len / 64;
	if (!(out = (unsigned char*)ft_memalloc(o_len)))
		ft_ssl_err("error");
	c = 0;
	while (c < newlines)
	{
		ft_memcpy(out + c * 65, in + c * 64, 64);
		out[(c + 1) * 64 + c] = '\n';
		c += 1;
	}
	ft_memcpy(out + c * 65, in + c * 64, *len % 64);
	free(in);
	out[o_len - 1] = o_len % 65 ? '\n' : 0;
	*len = o_len;
	return (out);
}

/*
** b64_normalize removes non basee64 characters from an input string
*/

void	*b64_normalize(void *final, char elem, size_t i, int *stop)
{
	(void)stop;
	(void)i;
	final = final ? final : (void*)ft_bufnew(ft_memalloc(10), 0, 10);
	if (IS_B64(elem))
		final = (void*)ft_bufappend((t_buf*)final, (void*)&elem, sizeof(char));
	return (final);
}

/*
** lookup_b64 finds the corresponding ascii from a given base64 encoded byte
*/

void	lookup_b64(unsigned char *tmp)
{
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (i < 4)
	{
		j = 0;
		while (j < 64)
		{
			if (tmp[i] == B64[j++])
			{
				tmp[i] = --j;
				break ;
			}
		}
		i += 1;
	}
}

/*
** b64_encode performs one round of encoding on the given base64 text,
** spreading information from 3 decoded charactess and
** translating them into 4 encoded characters
*/

void	b64_encode(t_base64 *ctx, unsigned char *in, unsigned char *out)
{
	int				i;
	unsigned char	buf[4];
	unsigned char	tmp[3];

	ft_memcpy((void*)tmp, (void*)in, 3);
	buf[0] = (tmp[0] & 0xfc) >> 2;
	buf[1] = ((tmp[0] & 0x03) << 4) + ((tmp[1] & 0xf0) >> 4);
	buf[2] = ((tmp[1] & 0x0f) << 2) + ((tmp[2] & 0xc0) >> 6);
	buf[3] = tmp[2] & 0x3f;
	i = -1;
	while (++i < 4)
		out[i] = B64[buf[i]];
	ctx->out_len += 4;
}

/*
** b64_decode performs one round of decoding on the given base64 text,
** collecting the information spread over 4 encoded charactess and
** translating them back into 3 decoded characters
*/

void	b64_decode(t_base64 *ctx, unsigned char *in, unsigned char *out)
{
	int				i;
	unsigned char	buf[3];
	unsigned char	tmp[4];

	ft_memcpy((void*)tmp, (void*)in, 4);
	i = -1;
	while (++i < 4)
	{
		if (tmp[i] == '=')
		{
			tmp[i] = '\0';
			ctx->out_len -= 1;
		}
	}
	lookup_b64(tmp);
	buf[0] = (tmp[0] << 2) + ((tmp[1] & 0x30) >> 4);
	buf[1] = ((tmp[1] & 0xf) << 4) + ((tmp[2] & 0x3c) >> 2);
	buf[2] = ((tmp[2] & 0x3) << 6) + tmp[3];
	i = -1;
	while (++i < 3)
		out[i] = buf[i];
	ctx->out_len += 3;
}
