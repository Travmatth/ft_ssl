/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b64.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/09 20:21:29 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/01 18:18:59 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

/*
** b64_init calculates and allocates the needed space for the resulting string,
** and, when decoding, normalizes the given input string, removing \n and other
** problem characters
*/

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
		free(b);
		ctx->in_len = LEN((char*)ctx->in, 0);
		len = LEN_FROM64(ctx->in_len);
	}
	if (!(ctx->out = (unsigned char*)ft_strnew(len)))
		ft_ssl_err("error");
}

/*
** b64_update performs one round of en/decoding to the given text
*/

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

/*
** b64_final finalizes the transformation process, alternatively adding padding
** characters to the encoded output or performing the last round of decoding
*/

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

/*
** b64_full encodes and decodes base64 strings and sets the resulting
** length in the *len variable. pass 1 for encoding, 0 for decoding
*/

unsigned char	*b64_full(unsigned char *in, size_t *len, int encoding)
{
	t_base64		ctx;
	size_t			inc;
	unsigned char	*out;
	unsigned char	*orig;

	ft_bzero(&ctx, sizeof(t_base64));
	b64_init(&ctx, in, *len, encoding);
	inc = encoding ? 3 : 4;
	orig = ctx.in;
	out = ctx.out;
	while (ctx.in_len >= inc)
	{
		b64_update(&ctx, ctx.in, ctx.out + ctx.out_len, encoding);
		ctx.in += inc;
		ctx.in_len -= inc;
	}
	b64_final(&ctx, ctx.in, ctx.out + ctx.out_len, encoding);
	if (encoding)
		out = b64_finalize(ctx.out, &ctx.out_len);
	free(orig);
	*len = ctx.out_len;
	return (out);
}

/*
** b64_wrapper performs the full process of encoding or decoding for ft_ssl
*/

void			b64_wrapper(void *input)
{
	t_b64			*ctx;
	unsigned char	*out;
	unsigned char	*orig;

	ctx = (t_b64*)input;
	if (!ctx->in)
		ctx->in = ft_memalloc(ctx->in_len);
	orig = ctx->in;
	out = b64_full(ctx->in, &ctx->in_len, GB64_ENCODE(ctx->mode));
	write(ctx->fd, out, ctx->in_len);
	if (GB64_DECODE(ctx->mode))
		free(orig);
}
