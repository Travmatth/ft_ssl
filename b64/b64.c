/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   b64.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/09 20:21:29 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/09 22:56:02 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

void	b64_init(t_base64 *ctx, unsigned char *in, size_t *len)
{
	ctx->in = in;
	// ctx->in = ft_strfoldl(b64_normalize, *len, (char*)in);
	// *len = GET_E(ctx->mode) ? LEN_FROM64(*len) : LEN_TO64(*len); 
	if (GET_E(ctx->mode))
		*len = LEN_TO64(*len); 
	else
		*len = LEN_FROM64(*len); 
	ctx->in_len = *len;
	if (!(ctx->out = (unsigned char*)ft_strnew(*len)))
		ft_ssl_err("error");
}

void	b64_update(t_base64 *ctx, unsigned char *in, unsigned char *out)
{
	if (GET_E(ctx->mode))
		b64_encode(ctx, in, out);
	else
		b64_decode(ctx, in, out);
}

void	b64_final(t_base64 *ctx, unsigned char *in, unsigned char *out)
{
	unsigned char	tmp[4];

	ft_memcpy((void*)out, (void*)in, ctx->in_len);
	ft_czero((void*)in, 0, 4 - ctx->in_len);
	if (GET_E(ctx->mode))
		b64_encode(ctx, tmp, out + ctx->out_len);
	else
		b64_decode(ctx, tmp, out + ctx->out_len);
}

unsigned char			*b64_full(unsigned char *in, size_t *len, int encoding)
{
	t_base64		*ctx;
	size_t			inc;
	unsigned char	*out;

	ctx = (t_base64*)ft_memalloc(sizeof(t_base64));
	if (encoding)
		SET_E(ctx->mode);
	b64_init(ctx, in, len);
	inc = GET_E(ctx->mode) ? 3 : 4;
	out = ctx->out;
	while (ctx->in_len >= inc)
	{
		b64_update(ctx, ctx->in, ctx->out + ctx->out_len);
		ctx->in += inc;
		ctx->in_len -= inc;
	}
	b64_final(ctx, ctx->in, ctx->out);
	return (out);
}

void	b64_wrapper(void *input)
{
	t_base64		*ctx;
	size_t			inc;
	unsigned char	*out;

	ctx = (t_base64*)input;
	b64_init(ctx, ctx->in, &ctx->in_len);
	inc = GET_E(ctx->mode) ? 3 : 4;
	out = ctx->out;
	while (ctx->in_len >= inc)
	{
		b64_update(ctx, ctx->in, ctx->out + ctx->out_len);
		ctx->in += inc;
		ctx->in_len -= inc;
	}
	b64_final(ctx, ctx->in, ctx->out);
	write(ctx->fd, out, ctx->out_len);
}
