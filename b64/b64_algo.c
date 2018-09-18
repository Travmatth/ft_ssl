/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b64_algo.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/09 20:21:40 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/17 18:35:24 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

void	*b64_normalize(void *final, char elem, size_t i, int *stop)
{
	(void)stop;
	(void)i;
	final = final ? final : (void*)ft_bufnew(ft_memalloc(10), 0, 10);
	if (IS_B64(elem))
		final = (void*)ft_bufappend((t_buf*)final, (void*)&elem, sizeof(char));
	return (final);
}

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
