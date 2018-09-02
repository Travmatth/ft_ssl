/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/13 11:01:21 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/01 13:33:24 by tmatthew         ###   ########.fr       */
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

uint64_t	ft_uint64to8(uint8_t block, uint8_t *out)
{
	uint8_t		i;

	i = 0;
	while (i < 8)
	{
		out[i] = (uint8_t)(block >> (8 * (8 (i + 1))));
		i += 1;
	}
}

uint64_t	permute_block(uint8_t *map, uint64_t block)
{
	(void)map;
	(void)block;
}

uint64_t	des_f(uint64_t	block, uint64_t key)
{
	int		i;
	uint64_t	permuted;
	uint8_t		inner;
	uint8_t		outer;

	permuted = 0;
	block = permute_block(g_des_exp, block);
	block ^= key;
	i = 0;
	while (i < 8)
	{
		outer = (((block >> (64 - (6 * i + 1))) & 1) << 1) | ((block >> (64 - (6 * i + 6)) & 1));
		inner = (block << (6 * i + 1)) >> 60;
		permuted |= (uint64_t)g_des_sboxes[i][outer][inner] << (60 - (4 * i));
		i += 1;
	}
	return (permute_block(g_des_pbox, permuted));
}

void		des_permute(t_desctx *ctx, uint64_t block, uint64_t keyschedule[16])
{
	uint8_t		i;
	uint64_t	left;
	uint64_t	right;

	i = 0;
	block = permute_block(g_init_perm, block);
	while (i < 16)
	{
		left = block << 32; 
		right = block >> 32; 
		right ^= (des_f(left, keyschedule) >> 32);
		block = left | right;
	}
	block = (block << 32) | (block >> 32);
	return (permute_block(g_des_final_perm, block));
}

void		key_operation_mode(int decrypt, uint64_t keyschedule[16])
{
	int			i;
	uint64_t	tmp;

	if (!decrypt)
		return ;
	i = 0;
	while (i < 16)
	{
		tmp = keyschedule[i];
		keyschedule[i] = keyschedule[15 - i];
		keyschedule[15 - i] = tmp;
		i += 1;
	}
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
	if (GET_DECRYPT(ctx->flags) && GET_A(ctx->flags))
	{
		ctx->plaintext = base64_decode((char*)ctx->plaintext, ctx->plen);
		ctx->plen = LEN(ctx->plaintext, 0);
	}
	while (i < 16)
	{
		left[i] = R_LEFT(left[i - 1], keyshift[i]);
		right[i] = R_LEFT(right[i - 1], keyshift[i]);
		key = ((uint64_t)left << sizeof(uint32_t)) | (uint64_t)right;
		keyschedule[i++] = permute_block(g_key_perm, key);
	}
	key_operation_mode(GET_DECRYPT(ctx->flags), keyschedule);
}


/*
** In cbc mode, we xor the translated 64bit block with the iv
** we also xor the permuted_block with the iv
** and also set as the newly created block the iv
*/

void		des_cbc_pre_permute_hook(t_desctx *ctx
	, uint64_t *block
	, uint8_t *plaintext
	, uint64_t keyschedule[16])
{
	(void)plaintext;
	(void)keyschedule;
	*block ^= ctx->init_vector;
}

void		des_cbc_post_permute_hook(t_desctx *ctx
	, uint64_t *block
	, uint8_t *plaintext
	, uint64_t keyschedule[16])
{
	(void)plaintext;
	(void)keyschedule;
	*block ^= ctx->init_vector;
	ctx->init_vector = *block; 
}

void		des_ecb_permute_hook(t_desctx *ctx
	, uint64_t *block
	, uint8_t *plaintext
	, uint64_t keyschedule[16])
{
	(void)ctx;
	(void)block;
	(void)plaintext;
	(void)keyschedule;
}

int			des_update(t_desctx *ctx
					, uint8_t *plaintext
					, uint64_t keyschedule[16])
{
	uint64_t	block;
	uint64_t	permuted_block;

	block = ft_uint8to64(ctx->plaintext);
	ctx->pre_permute_chaining(ctx, &block, plaintext, keyschedule);
	permuted_block = des_permute(ctx, block, plaintext, keyschedule);
	ctx->post_permute_chaining(ctx, &permute_block, keyschedule);
	ft_uint64to8(permute_block, ctx->ciphertext + ctx->clen);
	ctx->clen += 8;
	return (1);
}

int			des_final(t_desctx *ctx
					, uint8_t *plaintext
					, uint64_t keyschedule[16]
					, size_t plen)
{
	uint8_t	tmp[8];
	uint8_t	pad;
	ssize_t	rem;

	rem = ctx->plen % 8;
	if (rem && GET_NP(ctx->flags))
		ft_ssl_err("error");
	else if (rem)
	{
		ft_memcpy((void*)tmp, plaintext, rem * sizeof(uint8_t));
		pad = (uint8_t)(8 - rem);
		ft_czero(tmp + rem * sizeof(uint8_t), pad, pad);
		des_update(ctx, tmp, keyschedule);
	}
	if (GET_ENCRYPT(ctx->flags) && GET_A(ctx->flags))
	{
		ctx->ciphertext = base64_encode(ctx->ciphertext, ctx->clen);
		ctx->clen = LEN(ctx->ciphertext, 0);
	}
}

void		des_wrapper(void *input)
{
	t_desctx	*ctx;
	uint8_t		*plaintext;
	uint64_t	keyschedule[16];

	ctx  = (t_desctx*)input;
	ctx->key = ctx->key ? ctx->key : create_des_key(ctx);
	plaintext = ctx->plaintext;
	des_init(ctx, keyschedule);
	while (ctx->plen >= 8)
	{
		des_update(ctx, plaintext, keyschedule);
		plaintext += 8;
		ctx->plen -= 8;
	}
	des_final(ctx, plaintext, keyschedule, ctx->plen);
	free(ctx->plaintext);
	free(ctx);
}