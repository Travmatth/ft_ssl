/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/02 12:56:53 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/15 22:27:33 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

uint8_t	*prompt_for_password(void)
{
	char	*password;
	char	*verfication;

	password = getpass("enter des encryption password:");
	verfication = getpass("Verifying - enter des encryption password:");
	if (ft_strequ(password, verfication))
		return ((unsigned char*)password);
	else
		ft_ssl_err("error: passwords do not match");
}

void	create_des_key(t_desctx *ctx)
{
	uint8_t		result[64];
	uint8_t		key[64];
	t_scrypt	opts;

	ft_bzero(&opts, sizeof(t_scrypt));
	if (!(ctx->password))
		ctx->password = prompt_for_password();
	// scrypt(ctx, 16, 1, 1, result, 64);
	scrypt(ctx, opts);
	key = from_hex_hash(key, result, 64);
	write(ctx->out_file, key, ctx->k_len);
}

void			configure_des_params(t_desctx *ctx)
{
	size_t			o_len;
	unsigned char	*decoded;
	uint8_t			*orig;

	o_len = ctx->i_len + 8;
	if (!(ctx->out_text = (uint8_t*)ft_strnew(o_len)))
		ft_ssl_err("error");
	if (GET_DECRYPT(ctx->flags) && GET_A(ctx->flags))
	{
		orig = ctx->in_text;
		decoded = b64_full((unsigned char*)ctx->in_text, &ctx->i_len, 0);
		free(orig);
		ctx->in_text = (uint8_t*)decoded;
	}
}

/*
** des_wrapper_print manages the decoding of the given out_text
** as well as the formmatting of the output
*/

void			des_wrapper_print(t_desctx *ctx)
{
	unsigned char	*out;

	if (GET_A(ctx->flags) && GET_ENCRYPT(ctx->flags))
	{
		out = b64_full((unsigned char*)ctx->out_text, &ctx->o_len, 1);
		write(ctx->out_file, out, ctx->o_len);
		free(out);
	}
	else
		write(ctx->out_file, ctx->out_text, ctx->o_len);
	write(ctx->out_file, "\n", 1);
}
