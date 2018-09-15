/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scrypt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 19:22:59 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/15 14:18:40 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

# define SUCCESS 1
# define ERROR 0

void				verify_scrypt_params(unsigned parallelization_param 
	, unsigned block_size
	, unsigned cost_param)
{
	if (parallelization_param < 1
		|| (block_size < 1 || block_size >= (1UL << (31 - 7)) / block_size)
		|| (cost_param < 2 || ((cost_param & (cost_param - 1)) != 0  || cost_param >=  (1UL << (31 - 7)) / cost_param)))
		ft_ssl_err("error: incorrect scrypt configuration");
}

void				xor_word(uint32_t len, uint32_t *src, uint32_t *dst)
{
	len /= 8;
	while (len)
	{
		dst[0] ^= src[0];
		dst[1] ^= src[1];
		dst[2] ^= src[2];
		dst[3] ^= src[3];
		dst[4] ^= src[4];
		dst[5] ^= src[5];
		dst[6] ^= src[6];
		dst[7] ^= src[7];
		len -= 1;
		dst += 8;
		src += 8;
	}
}

void				scryptBlockMix(uint32_t block_size
	, uint32_t *blocks
	, uint32_t *mixed)
{
	uint32_t	internal[16];
	uint32_t	i;

	block_size *= 2;
		// #define movw(w, S, D)	memmove(D, S, (w)*4)
	ft_memmove((void*)internal, &blocks[(block_size - 1) * 16], 16 * 4);
	i = 0;
	while (i < block_size)
	{
		xor_word(16, &blocks[i * 16], internal);
		salsa_core(internal, internal, 8);
		ft_memmove((void*)&mixed[i * 8], (void*)internal, 16 * 4);

		xor_word(16, &blocks[(i + 1) * 16], internal);
		salsa_core(internal, internal, 8);
		ft_memmove((void*)&mixed[i * 8 + block_size * 8], (void*)internal, 16 * 4);
		i += 2;
	}
}

/*
** The scryptROMix Algorithm
**    The scryptROMix algorithm is the same as the ROMix algorithm
**    described in [SCRYPT] but with scryptBlockMix used as the hash
**    function H and the Integerify function explained inline.
**    Algorithm scryptROMix
** Input:
** r       Block size parameter.
** B       Input octet vector of length 128 * r octets.
** N       CPU/Memory cost parameter, must be larger than 1,
**         a power of 2, and less than 2^(128 * r / 8).
** Output:
** B’      Output octet vector of length 128 * r octets.
*/

void		scryptROMix(unsigned block_size // r
	, unsigned cost_param
	, uint32_t *block_scratch_space
	, uint32_t *cost_scratch_space
	, unsigned char *blocks)
{
	uint32_t	len;
	uint32_t	i;
	uint32_t	desired;
	uint32_t	*internal_scratch_space;

	i = 0;
	len = block_size * 32;
	while (i < len)
	{
		cost_scratch_space[i] = blocks[0]
			| (blocks[1] << 8)
			| (blocks[2] << 16)
			| (blocks[3] << 24);
		blocks += 4;
		i += 1;
	}
	internal_scratch_space = &cost_scratch_space[len];
	i = 0;
	while (i < cost_param)
	{
		ft_memmove(&block_scratch_space[i * len], cost_scratch_space, len * 4);
		scryptBlockMix(block_size, cost_scratch_space, internal_scratch_space);
		ft_memmove(&block_scratch_space[(i + 1) * len], internal_scratch_space, len * 4);
		scryptBlockMix(block_size, internal_scratch_space, cost_scratch_space);
		i += 2;
	}
	i = 0;
	while (i < cost_param)
	{
		// xorw(w, &V[(X[w-16] & (N-1))*w], X);
		// xorw(ulong w, u32int *S, u32int *D)
		xor_word(len, &block_scratch_space[(cost_scratch_space[len - 16] & (cost_param - 1)) * len], cost_scratch_space);
		scryptBlockMix(block_size, cost_scratch_space, internal_scratch_space);
		xor_word(len, &block_scratch_space[(internal_scratch_space[len - 16] & (cost_param - 1)) * len], internal_scratch_space);
		scryptBlockMix(block_size, internal_scratch_space, cost_scratch_space);
		i += 2;
	}
	blocks -= len * 4;
	i = 0;
	while (i < len)
	{
		desired = cost_scratch_space[i];
		blocks[0] = desired;
		blocks[1] = desired >> 8;
		blocks[2] = desired >> 16;
		blocks[3] = desired >> 24;		
		blocks += 4;
		i += 1;
	}
}

/*
** http://www.tarsnap.com/scrypt/scrypt.pdf
** P passphrase, an octet string
** S Salt, an octet string
** r blockSize specifies the block size.
** N CPU/Memory cost param, must be larger than 1, a power of 2, and less than 2^(128 * r / 8).
** p parallelization param, a positive integer less than or equal to ((2^32-1) * hLen) / MFLen
** 		where hLen is 32 and MFlen is 128 * r.
** dkLen, intended output length in octets of the derived key;
**      a positive integer less than or equal to (2^32 - 1) * hLen where hLen is 32.
** outputs: DK, derived key of length dkLen
*/

void		scrypt(unsigned char *passphrase //p
	, size_t passphrase_len //i_len
	, unsigned char *salt //s
	, size_t salt_len //slen
	, unsigned cost_param // N
	, unsigned block_size //R
	, unsigned parallelization_param //P
	, unsigned char *key //d
	, size_t key_len) //dlen
{
	size_t			i;
	unsigned char	*blocks;
	uint32_t		*block_scratch_space;
	uint32_t		*cost_scratch_space;
	uint32_t rb = block_size << 7;

	verify_scrypt_params(parallelization_param, block_size, cost_param); 
	// uses pbkdf2 with pseudorandom func PRF to generate p blocks
	// of length MFLen octets from the provided password/salt
	// (B0 . . . Bp−1) ← PBKDF2_PRF (P, S, 1, p · MF Len)
	// block_len = 128 * block_size;
	// Need to pull these inits out into separate function
	// if allocs fail, need to free prev
	blocks = ft_memalloc(parallelization_param * rb);
	block_scratch_space = ft_memalloc(cost_param * rb);
	cost_scratch_space = ft_memalloc(2 * rb);
	// TEST SHIM
	// (void)salt;
	// (void)salt_len;
	// (void)key;
	// uint8_t	k[64];
	// ALL PBKDF2 TESTS PASSING
	// pbkdf2((uint8_t*)"password", 8, (uint8_t*)"salt", 4, k, 32, 1);
	// pbkdf2((uint8_t*)"password", 8, (uint8_t*)"salt", 4, k, 32, 2);
	// pbkdf2((uint8_t*)"password", 8, (uint8_t*)"salt", 4, k, 32, 4096);
	// pbkdf2((uint8_t*)"password", 8, (uint8_t*)"salt", 4, k, 32, 16777216);
	// pbkdf2((uint8_t*)"passwordPASSWORDpassword", 24 , (uint8_t*)"saltSALTsaltSALTsaltSALTsaltSALTsalt", 36 , k, 40, 4096);
	// pbkdf2((uint8_t*)"pass\0word", 9 , (uint8_t*)"sa\0lt", 5 , k, 16, 4096);
	// SALSA20/8 TESTS

	// TEST SHIM
	pbkdf2(passphrase, passphrase_len, salt, salt_len, blocks, 128 * block_size * parallelization_param, 1);


	// independently mixed using mixing func scryptROMix
	i = 0;
	while (i < parallelization_param)
	{
		scryptROMix(block_size, cost_param, block_scratch_space, cost_scratch_space, &blocks[i * 128 * block_size]);
		i += 1;
	}

	// final output is generated by applying pbkdf2 again,
	// using well-mixed blocks as salt. 
	pbkdf2(passphrase, passphrase_len, blocks, i * 128 * block_size, key, key_len, 1);
	free(blocks);
	free(block_scratch_space);
	free(cost_scratch_space);
}
