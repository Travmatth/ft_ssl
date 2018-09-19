/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scrypt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 19:22:59 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/19 11:47:25 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

/*
** scrypt_block_mix mixes given input blocks through rounds of
** xor'ing and mixing using salsa_core
*/

void	scrypt_block_mix(uint32_t block_size
	, uint32_t *blocks
	, uint32_t *mixed)
{
	uint32_t	internal[16];
	uint32_t	i;

	block_size *= 2;
	ft_memmove((void*)internal, &blocks[(block_size - 1) * 16], 16 * 4);
	i = 0;
	while (i < block_size)
	{
		xor_word(16, &blocks[i * 16], internal);
		salsa_core(internal, internal, 8);
		ft_memmove((void*)&mixed[i * 8], (void*)internal, 16 * 4);
		xor_word(16, &blocks[(i + 1) * 16], internal);
		salsa_core(internal, internal, 8);
		ft_memmove((void*)&mixed[i * 8 + block_size * 8]
			, (void*)internal
			, 16 * 4);
		i += 2;
	}
}

/*
** The scrypt_ro_mix Algorithm
**    The scrypt_ro_mix algorithm is the same as the ROMix algorithm
**    described in [SCRYPT] but with scrypt_block_mix used as the hash
**    function H and the Integerify function explained inline.
**    Algorithm scrypt_ro_mix
** Input:
** r       Block size parameter.
** B       Input octet vector of length 128 * r octets.
** N       CPU/Memory cost parameter, must be larger than 1,
**         a power of 2, and less than 2^(128 * r / 8).
** Output:
** B’      Output octet vector of length 128 * r octets.
*/

void	scrypt_ro_mix(t_scrypt *opts
	, uint32_t *block_tmp
	, uint32_t *cost_tmp
	, unsigned char *blocks)
{
	uint32_t	len;
	uint32_t	i;
	uint32_t	*internal;

	i = 0;
	to_little_endian(blocks, cost_tmp, (len = opts->block_size * 32));
	internal = &cost_tmp[len];
	while (i < opts->cost_param)
	{
		ft_memmove(&block_tmp[i * len], cost_tmp, len * 4);
		scrypt_block_mix(opts->block_size, cost_tmp, internal);
		ft_memmove(&block_tmp[(i + 1) * len], internal, len * 4);
		scrypt_block_mix(opts->block_size, internal, cost_tmp);
		i += 2;
	}
	i = 0;
	while (i < opts->cost_param)
	{
		xor_word(len, &block_tmp[BL_OFFSET(cost_tmp, len, opts)], cost_tmp);
		scrypt_block_mix(opts->block_size, cost_tmp, internal);
		xor_word(len, &block_tmp[BL_OFFSET(internal, len, opts)], internal);
		scrypt_block_mix(opts->block_size, internal, cost_tmp);
		i += 2;
	}
	to_big_endian(blocks, cost_tmp, len);
}

/*
** set scrypt options
*/

void	configure_opts(t_scrypt *opts
					, t_desctx *ctx
					, unsigned char *blocks
					, size_t i)
{
	if (!i)
	{
		opts->password = ctx->password;
		opts->p_len = ctx->p_len;
		opts->salt = ctx->salt;
		opts->s_len = ctx->s_len;
		opts->key = blocks;
		opts->k_len = opts->parallel_param * (opts->block_size << 7);
		return ;
	}
	opts->password = ctx->password;
	opts->p_len = ctx->p_len;
	opts->salt = blocks;
	opts->s_len = opts->parallel_param * (opts->block_size << 7);
	opts->key = ctx->key;
	opts->k_len = ctx->k_len;
}

/*
** http://www.tarsnap.com/scrypt/scrypt.pdf
** P passphrase, an octet string
** S Salt, an octet string
** r blockSize specifies the block size.
** N CPU/Memory cost param, must be larger than 1
** 		, a power of 2, and less than 2^(128 * r / 8).
** p parallelization param, a positive integer less than or equal to
** 		((2^32-1) * hLen) / MFLen
** 		where hLen is 32 and MFlen is 128 * r.
** dkLen, intended output length in octets of the derived key;
**      a positive integer less than or equal to (2^32 - 1) * hLen
**		where hLen is 32.
** outputs: DK, derived key of length dkLen
**
** uses pbkdf2 with pseudorandom func PRF to generate p blocks
** of length MFLen octets from the provided password/salt
** (B0 . . . Bp−1) ← PBKDF2_PRF (P, S, 1, p · MF Len)
** block_len = 128 * block_size;
** independently mixed using mixing func scrypt_ro_mix
** final output is generated by applying pbkdf2 again,
** using well-mixed blocks as salt.
*/

void	scrypt(t_desctx *ctx, t_scrypt *opts)
{
	size_t			i;
	unsigned char	*blocks;
	uint32_t		*block_tmp;
	uint32_t		*cost_tmp;
	size_t			rb;

	i = 0;
	rb = opts->block_size << 7;
	verify_scrypt_params(opts->parallel_param
						, opts->block_size, opts->cost_param);
	blocks = ft_memalloc(opts->parallel_param * rb);
	block_tmp = ft_memalloc(opts->cost_param * rb);
	cost_tmp = ft_memalloc(2 * rb);
	configure_opts(opts, ctx, blocks, i);
	pbkdf2(opts, 1);
	while (i < opts->parallel_param)
		scrypt_ro_mix(opts, block_tmp, cost_tmp, &blocks[i++ * rb]);
	configure_opts(opts, ctx, blocks, i);
	pbkdf2(opts, 1);
	free(blocks);
	free(block_tmp);
	free(cost_tmp);
}
