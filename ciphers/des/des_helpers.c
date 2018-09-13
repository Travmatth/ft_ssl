/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/02 12:56:53 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/12 22:33:08 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

unsigned char		*prompt_for_password(void)
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

uint8_t				*create_des_key(t_desctx *ctx)
{
	// int			fd;
	uint8_t		*key;
	uint8_t		result[64];
	// uint64_t	buf[2];

	// TESTING SHIM
	// if (!ctx->password)
	// 	ctx->password = prompt_for_password();
	// if (ctx->salt[0] == 0 && ctx->salt[1] == 0)
	// {
	// 	if (!(fd = open("/dev/random", O_RDONLY)))
	// 		ft_ssl_err("error: cannot open /dev/random");
	// 	if (!read(fd, buf, sizeof(uint64_t) * 2))
	// 		ft_ssl_err("error occurred while reading /dev/random");
	// 	ft_memcpy(ctx->salt, (void*)buf, sizeof(uint64_t));
	// }
	// TESTING SHIM
	// scrypt(ctx->password, ctx->i_len, ctx->salt, ctx->s_len, 16, 1, 1, result, 64);
	scrypt((unsigned char*)"", 0, (unsigned char*)"", 0, 16, 1, 1, result, 64);
	// TESTING SHIM
	if (!(key = ft_memalloc(ctx->k_len)))
		ft_ssl_err("error");
	ft_memcpy((void*)&key, (void*)result, sizeof(uint64_t));
	return (key);
}

void				configure_des_params(t_desctx *ctx)
{
	size_t			o_len;
	unsigned char	*decoded;

	o_len = ctx->i_len + 8;
	if (!(ctx->out_text = (uint8_t*)ft_strnew(o_len)))
		ft_ssl_err("error");
	// TESTING SHIM
	create_des_key(ctx);
	// TESTING SHIM
	if (!ctx->key)
	{
		ctx->k_len = 8;
		ctx->key = create_des_key(ctx);
	}
	if (GET_DECRYPT(ctx->flags) && GET_A(ctx->flags))
	{
		decoded = b64_full((unsigned char*)ctx->in_text, &ctx->i_len, 0);
		ctx->in_text = (uint8_t*)decoded;
	}
}
