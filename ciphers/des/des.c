/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/13 11:01:21 by tmatthew          #+#    #+#             */
/*   Updated: 2018/08/31 15:51:20 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"
#include "../../includes/des_constants.h"

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

unsigned char		*create_des_key(t_desctx *ctx)
{
	int			fd;
	char		*key;
	unsigned char		result[64];
	uint64_t	buf[2];

	if (!ctx->password)
		ctx->password = prompt_for_password();
	if (ctx->salt[0] == 0 && ctx->salt[1] == 0)
	{
		if (!(fd = open("/dev/random", O_RDONLY)))
			ft_ssl_err("error: cannot open /dev/random");
		if (!read(fd, buf, sizeof(uint64_t) * 2))
			ft_ssl_err("error occurred while reading /dev/random");
		ft_memcpy(ctx->salt, (void*)buf, sizeof(uint64_t));
	}
	scrypt(ctx->password, ctx->plen, ctx->salt, ctx->slen, 16, 1, 1, result, 64);
	ft_memcpy((void*)&key, (void*)result, sizeof(uint64_t));
	return ((unsigned char*)key);
}

int			ft_htouint64(uint8_t *hex, uint64_t *val)
{
	uint8_t		c;

	*val = 0;
	while (i < 16)
	{
		c = hex[i++];
		*val *= 16;
		if (c >= 'A' && c <= 'F')
			*val += c - 'A' + 10;
		else if (c >= 'a' && c <= 'f')
			*val += c - 'a' + 10;
		else if (c >= '0' && c <= '9')
			*val += c - '0';
		else
			return (0);
	}
	return (1);
}

uint64_t	ft_uint8to64(uint8_t *block)
{
	uint8_t		i;
	uint64_t	new;

	new = 0;
	i = 0;
	while (i < 8)
	{
		new |= ((uint64_t)block[i]) << (8 * (8 (i + 1)));
		i += 1;
	}
	return (new);
}

uint64_t	permute_block(uint8_t *map, uint64_t block)
{
	(void)map;
	(void)block;
}

int			des_init(t_desctx *ctx, uint64_t keyschedule[16])
{
	uint8_t		i;
	uint64_t	key;
	uint32_t	left[16];
	uint32_t	right[16];
	uint8_t		keyshift[16];

	if (!ft_htouint64(ctx->key, &key))
		ft_ssl_err("error: invalid key");
	i = 0;
	left[0] = key;
	right[0] = key << 28;
	keyshift = GET_DECRYPT(ctx->flags) ? g_des_key_dec : g_des_key_enc;
	while (i < 16)
	{
		left[i] = R_LEFT(left[i - 1], keyshift[i]);
		right[i] = R_LEFT(right[i - 1], keyshift[i]);
		key = ((uint64_t)left << sizeof(uint32_t)) | (uint64_t)right;
		keyschedule[i++] = permute_block(g_key_perm, key);
	}
	// set enc/ dec here
}

int			des_update(t_desctx *ctx, uint64_t keyschedule[16])
{
	uint64_t	block;

	block = ft_uint8to64(ctx->plaintext);
	//for cbc/ofb/etc
	//block = ctx->block_mode(ctx, block);
}

int			des_final(t_desctx *ctx, uint64_t keyschedule[16])
{
}

void		des_wrapper(void *input)
{
	t_desctx	*ctx;
	uint8_t		*orig;
	uint64_t	keyschedule[16];

	ctx = (t_desctx*)input;
	ctx->key = ctx->key ? ctx->key : create_des_key(ctx);
	orig = ctx->plaintext;
	des_init(ctx, keyschedule);
	while (ctx->plen > 8)
	{
		des_update(ctx, keyschedule);
		ctx->plaintext += 8;
		ctx->plen -= 8;
	}
	des_final(ctx, keyschedule);
	free(orig);
}