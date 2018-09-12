/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/13 11:01:23 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/11 19:11:14 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

/*
** You must include the following flags for DES:
** • -a, decode/encode the input/output in base64, depending on the encrypt mode
** • -d, decrypt mode
** • -e, encrypt mode (default)
** • -i, input file for message
** • -k, key in hex is the next arguement.
** (Behave like openssl des -K not openssl des -k)
** • -o, output file for message
** • -p, password in ascii is the next argument.
** (Behave like a modifiedd openssl des -pass not like openssl des -p or -P)
** • -s, the salt in hex is the next argument.
** (Behave like openssl des -S)
** • -v, initialization vector in hex is the next argument.
** (Behave like openssl des -iv not openssl des -v)
** If flags are not provided, be prepared to read/write from/to the console for the missing
** parameters similar to the behavior of OpenSSL.
**
** When the user does not have a cryptographically secure key, a new one must be created.
** This is why when a key is not provided, OpenSSL asks the user for a password. The key
** is generated using a Password-Based Key Derivation Function, or PBKDF.
**
** > openssl des-ecb
** enter des-ecb encryption password:
**
** To see the key that is generated from the password, you can use the
** -P flag.
** > openssl des-ecb -P
** enter des-ecb encryption password:
** Verifying - enter des-ecb encryption password:
** salt=EDCFEEFCA1850351
** key=914A103B0CE0A235
** >
** > openssl des-ecb -P
** enter des-ecb encryption password:
** Verifying - enter des-ecb encryption password:
** salt=3EB317A13C39A7D8
** key=DD334A3DE9C4C449
**
** To make your own keys from passwords, you will have to implement your own PBKDF.
** You must read data from STDIN (using your choice of the allowed functions above to
** prevent it showing up on the terminal) or read it as a flag. Your flag will of course be
** much cleaner than that of OpenSSL.
**
** > openssl des-ecb -pass "pass:MySuperSecurePassword"
** > ./ft_ssl des-ecb -p "MySuperSecurePassword"
**
** RFC 2898 sets PBKDF2 as the standard and RFC 8018 still endorses it.
** Another hint: re-creating OpenSSL’s PBKDF will not be very hard:
** it’s not really secure. Really.
**
** • A 64-bit long key. If a key is too short, pad it with zeros.
** Longer keys are truncated with the remainder discarded.
*/

/*
** parses next option and modifies ctx with flag or string to be hashed
*/

void	parse_base64_opts_handler(t_desctx *ctx, char **argv, int *i)
{
	int		fd;

	if (ft_strequ("des-ecb", argv[*i]))
	{
		ctx->pre_permute_chaining = des_null_permute_hook;
		ctx->post_permute_chaining = des_null_permute_hook;
	}
	else if (ft_strequ("des-cbc", argv[*i]))
	{
		ctx->pre_permute_chaining = des_cbc_pre_permute_hook;
		ctx->post_permute_chaining = des_cbc_post_permute_hook;
	}
	// else if (ft_strequ("--nopad", argv[*i]))
	// 	SET_A(ctx->flags);
	else if (ft_strequ("-a", argv[*i]))
		SET_A(ctx->flags);
	else if (ft_strequ("-d", argv[*i]))
		SET_DECRYPT(ctx->flags);
	else if (ft_strequ("-e", argv[*i]))
		SET_ENCRYPT(ctx->flags);
	else if (ft_strequ("-i", argv[*i]))
	{
		if (!argv[*i + 1])
			ft_ssl_err("error");
		if (!(fd = open(argv[*i + 1], O_RDONLY)))
			ft_ssl_err("error: cannot find file");
		ctx->plaintext = (uint8_t*)ft_str_from_fd(fd);
		ctx->plen = LEN((char*)ctx->plaintext, 0);
		*i += 1;
		SET_INPUT(ctx->flags);
	}
	else if (ft_strequ("-k", argv[*i]))
	{
		if (!argv[*i + 1])
			ft_ssl_err("error");
		ctx->key = (uint8_t*)ft_strdup(argv[*i + 1]);
		ctx->klen = LEN((char*)ctx->key, 0);
		*i += 1;
	}
	else if (ft_strequ("-o", argv[*i]))
	{
		if (!argv[*i + 1]
			|| ERR(ctx->out_file = open(argv[*i + 1], O_WRONLY)))
			ft_ssl_err("error");
		*i += 1;
	}
	else if (ft_strequ("-p", argv[*i]))
	{
		if (!argv[*i + 1])
			ft_ssl_err("error");
		ctx->password = (uint8_t*)ft_strdup(argv[*i + 1]);
		ctx->plen = LEN((char*)ctx->password, 0);
		*i += 1;
	}
	else if (ft_strequ("-s", argv[*i]))
	{
		if (!argv[*i + 1])
			ft_ssl_err("error");
		ctx->salt = (uint8_t*)ft_strdup(argv[*i + 1]);
		ctx->slen = LEN((char*)ctx->salt, 0);
		*i += 1;
	}
	else if (ft_strequ("-v", argv[*i]))
	{
		if (!argv[*i + 1])
			ft_ssl_err("error");
		if (!(ft_htouint64((uint8_t*)(argv[*i + 1]), &ctx->init_vector)))
			ft_ssl_err("error: invalid init vector");
		*i += 1;
	}
	// Need to handle -P a la pdf section v.03
}

/*
** accepts the argc & argv arguments, parses and returns
** a struct holding string to be hashed
*/

void		*parse_des_opts(int argc, char **argv)
{
	int			i;
	t_desctx	ctx;
	void		*new;

	i = -1;
	ft_bzero(&ctx, sizeof(t_desctx));
	while (++i < argc)
		parse_base64_opts_handler(&ctx, argv, &i);
	if (!GET_DECRYPT(ctx.flags) && !GET_ENCRYPT(ctx.flags))
		SET_ENCRYPT(ctx.flags);
	if (!ctx.out_file)
		ctx.out_file = STDOUT;
	if (!GET_INPUT(ctx.flags))
	{
		ctx.plaintext = (uint8_t*)ft_str_from_fd(STDIN);
		ctx.plen = LEN((char*)ctx.plaintext, 0);
	}
	if (!(new = ft_memalloc(sizeof(t_desctx))))
		ft_ssl_err("error");
	return (ft_memcpy(new, &ctx, sizeof(t_desctx)));
}
