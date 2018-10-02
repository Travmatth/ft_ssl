/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/02 12:56:53 by tmatthew          #+#    #+#             */
/*   Updated: 2018/10/02 11:02:41 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

static char	*g_ask = "enter des encryption password: ";
static char	*g_challenge = "Verifying - enter des encryption password: ";

/*
** configure_key_params performs necessary setup for the des operation
** allocating variables needed and editing inputs
*/

void	configure_key_params(t_desctx *ctx, char *salt)
{
	int				fd;
	unsigned char	buf[3 * sizeof(uint64_t)];
	size_t			desired;

	desired = (GET_TRIPLE(ctx->flags) ? 3 : 1) * sizeof(uint64_t);
	if (!ctx->password && (ctx->password = (uint8_t*)getpass(g_ask)))
		if (!ft_strequ((char*)ctx->password, getpass(g_challenge)))
			ft_ssl_err("error: passwords do not match");
	if (!ctx->salt)
	{
		if (ERR((fd = open("/dev/random", O_RDONLY))))
			ft_ssl_err("error");
		if (read(fd, buf, desired))
			ctx->salt = (uint8_t*)from_hex(salt, buf, desired);
		else
			ft_ssl_err("error");
		ctx->s_len = LEN((char*)ctx->salt, 0);
		close(fd);
	}
	if (!ctx->key)
	{
		ctx->k_len = desired;
		if (!(ctx->key = ft_memalloc(ctx->k_len)))
			ft_ssl_err("error");
	}
}

/*
** create_des_key creates a key from a given password / salt
** using scrypt as the pbkdf
*/

void	create_des_key(t_desctx *ctx)
{
	char		salt[65];
	char		key[65];
	t_scrypt	opts;

	ft_bzero(&opts, sizeof(t_scrypt));
	opts.block_size = 1;
	opts.cost_param = 16;
	opts.parallel_param = 1;
	configure_key_params(ctx, salt);
	scrypt(ctx, &opts);
	ctx->key = (uint8_t*)from_hex_hash(key, ctx->key, ctx->k_len);
	ctx->k_len = LEN((char*)ctx->key, 0);
	ft_printf("salt=%s\nkey=%s\n", ctx->salt, ctx->key);
}

/*
** adjust_key performs truncation on given key, if necessary
*/

void	adjust_key(t_desctx *ctx, const char *mode)
{
	uint8_t			*orig;

	if (ft_strequ("des", mode))
	{
		orig = ctx->key;
		if (ctx->k_len < 16)
			ctx->key = (uint8_t*)ft_memcpy(ft_memalloc(16), orig, ctx->k_len);
		else
			return ;
		ft_memset(ctx->key + ctx->k_len, '0', 16 - ctx->k_len);
		ctx->k_len = 16;
		free(orig);
		return ;
	}
	orig = ctx->key;
	if (ctx->k_len < 48)
		ctx->key = (uint8_t*)ft_memcpy(ft_memalloc(48), orig, ctx->k_len);
	else
		return ;
	ft_memset(ctx->key + ctx->k_len, '0', 48 - ctx->k_len);
	ctx->k_len = 48;
	free(orig);
}

/*
** configure_des_params performs necessary setup for the des operation
** allocating variables needed and editing inputs
*/

void	configure_des_params(t_desctx *ctx, const char *mode)
{
	size_t			o_len;
	unsigned char	*decoded;
	uint8_t			*orig;

	o_len = ctx->i_len + 8;
	if (!(ctx->out_text = (uint8_t*)ft_strnew(o_len)))
		ft_ssl_err("error");
	if (!ctx->in_text)
	{
		ctx->in_text = (uint8_t*)ft_memalloc(0);
		ctx->i_len = 0;
	}
	if (GET_DECRYPT(ctx->flags) && GET_A(ctx->flags))
	{
		orig = ctx->in_text;
		decoded = b64_full((unsigned char*)ctx->in_text, &ctx->i_len, 0);
		free(orig);
		ctx->in_text = (uint8_t*)decoded;
	}
	if (!GET_K(ctx->flags))
		adjust_key(ctx, mode);
}

/*
** des_wrapper_print manages the decoding of the given out_text
** as well as the formmatting of the output
*/

void	des_wrapper_print(t_desctx *ctx)
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
	free(ctx->key);
	free(ctx->in_text);
}
