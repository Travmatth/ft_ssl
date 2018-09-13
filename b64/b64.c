/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b64.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/09 20:21:29 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/12 20:12:45 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

void			b64_init(t_base64 *ctx
						, unsigned char *in
						, size_t in_len
						, int encoding)
{
	size_t	len;
	t_buf	*b;

	ctx->in = in;
	ctx->in_len = in_len;
	if (encoding)
		len = ((4 * in_len / 3) + 3) & ~3;
	else
	{
		b = (t_buf*)ft_strfoldl(b64_normalize, in_len, (char*)in);
		ctx->in = (unsigned char*)b->buf;
		ctx->in_len = LEN((char*)ctx->in, 0);
		len = LEN_FROM64(ctx->in_len);
	}
	if (!(ctx->out = (unsigned char*)ft_strnew(len)))
		ft_ssl_err("error");
}

void			b64_update(t_base64 *ctx
						, unsigned char *in
						, unsigned char *out
						, int encoding)
{
	if (encoding)
		b64_encode(ctx, in, out);
	else
		b64_decode(ctx, in, out);
}

void			b64_final(t_base64 *ctx
						, unsigned char *in
						, unsigned char *out
						, int encoding)
{
	ptrdiff_t		rem;
	unsigned char	tmp[4];

	if (!ctx->in_len)
		return ;
	rem = 3 - ctx->in_len;
	ft_memcpy((void*)tmp, (void*)in, ctx->in_len);
	ft_bzero((void*)(tmp + ctx->in_len), rem);
	if (encoding)
	{
		b64_encode(ctx, tmp, out);
		while (rem)
			*(ctx->out + ctx->out_len - rem--) = '=';
	}
	else
		b64_decode(ctx, tmp, out);
}

unsigned char	*b64_full(unsigned char *in, size_t *len, int encoding)
{
	t_base64		*ctx;
	size_t			inc;
	unsigned char	*out;

	if (!(ctx = (t_base64*)ft_memalloc(sizeof(t_base64))))
		ft_ssl_err("error");
	b64_init(ctx, in, *len, encoding);
	inc = encoding ? 3 : 4;
	out = ctx->out;
	while (ctx->in_len >= inc)
	{
		b64_update(ctx, ctx->in, ctx->out + ctx->out_len, encoding);
		ctx->in += inc;
		ctx->in_len -= inc;
	}
	b64_final(ctx, ctx->in, ctx->out + ctx->out_len, encoding);
	*len = ctx->out_len;
	return (out);
}

void			b64_wrapper(void *input)
{
	t_b64			*ctx;
	unsigned char	*out;

	ctx = (t_b64*)input;
	if (!ctx->in)
		ctx->in = ft_memalloc(ctx->in_len);
	out = b64_full(ctx->in, &ctx->in_len, GET_E(ctx->mode));
	write(ctx->fd, out, ctx->in_len);
}
