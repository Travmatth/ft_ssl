/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/30 20:13:01 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/12 23:07:26 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ssl.h"

/*
** Four 32-bit variables are initialized:
** A = 0x01234567
** B = 0x89abcdef
** C = 0xfedcba98
** D = 0x76543210
** These are called chaining_variables.
** main_loop -> while 512-bit blocks remaining
** a, b, c, d = A, B, C, D
** rounds -> for 1..4
** 	apply different op 16 times, each taking 3/4 of a, b, c, d
** 	add result of prev to fourth var, a sub_block of text and constant
** 	rotate result to right a variable # of bits
** 	adds result to one of a, b, c, or d
** 	result replaces one of a, b, c, or d
** op can be one of four funcs
** 	F(X,Y,Z) = (X ¥ Y) ¦ ((¬ X) ¥ Z)
** 	G(X,Y,Z) = (X ¥ Z) ¬ (Y (¬ Z))
** 	H(X,Y,Z) = X • Y • Z
** 	I(X,Y,Z) = Y • (X ¦ (¬ Z))
** 	(• is XOR,¥ is AND, ¦ is OR, and ¬ is NOT.)
** If Mj represents the j th sub-block of the message (from 0 to 15), and <<<s
** represents a left circular shift of s bits, the four operations are:
** 	FF(a,b,c,d,Mj,s,ti) denotes a = b + ((a + F(b,c,d ) + Mj+ ti) <<< s)
** 	GG(a,b,c,d,Mj,s,ti) denotes a = b + ((a + G(b,c,d ) + Mj+ ti) <<< s)
** 	HH(a,b,c,d,Mj,s,ti) denotes a = b + ((a + H(b,c,d) + Mj+ ti) <<< s)
** 	II(a,b,c,d,Mj,s,ti) denotes a = b + ((a + I(b,c,d ) + Mj+ ti) <<< s)
** 
** 
** 	four rounds:
** 		FF, GG, HH, II
** 		each takes (a, b, c, d, M_x, 7|12|17|22, chaining_variable?)
** 			the order of a, b, c, d is rotated once per application
** 				can use modulo to check order
** 			x  increases by 1 per application
** 				use union?
** 			what to call 7|112|17|22, can we tie them to param order modulo?
** 			t_i is radian_lookup_table[i], can we tie them to x?
** 		
** 	The four rounds (64 steps) look like:
** 	Round 1:
** 	FF (a, b, c, d, M0, 7, 0xd76aa478)
** 	FF (d, a, b, c, M1, 12, 0xe8c7b756)
** 	FF (c, d, a, b, M2, 17, 0x242070db)
** 	FF (b, c, d, a, M3, 22, 0xc1bdceee)
** 	FF (a, b, c, d, M4, 7, 0xf57c0faf)
** 	FF (d, a, b, c, M5, 12, 0x4787c62a)
** 	FF (c, d, a, b, M6, 17, 0xa8304613)
** 	FF (b, c, d, a, M7, 22, 0xfd469501)
** 	FF (a, b, c, d, M8, 7, 0x698098d8)
** 	FF (d, a, b, c, M9, 12, 0x8b44f7af)
** 	FF (c, d, a, b, M10, 17, 0xffff5bb1)
** 	FF (b, c, d, a, M11, 22, 0x895cd7be)
** 	FF (a, b, c, d, M12, 7, 0x6b901122)
** 	FF (d, a, b, c, M13, 12, 0xfd987193)
** 	FF (c, d, a, b, M14, 17, 0xa679438e)
** 	FF (b, c, d, a, M15, 22, 0x49b40821)
** 
** 	Round 2:
** 	GG (a, b, c, d, M1, 5, 0xf61e2562)
** 	GG (d, a, b, c, M6, 9, 0xc040b340)
** 	GG (c, d, a, b, M11, 14, 0x265e5a51)
** 	GG (b, c, d, a, M0, 20, 0xe9b6c7aa)
** 	GG (a, b, c, d, M5, 5, 0xd62f105d)
** 	GG (d, a, b, c, M10, 9, 0x02441453)
** 	GG (c, d, a, b, M15, 14, 0xd8a1e681)
** 	GG (b, c, d, a, M4, 20, 0xe7d3fbc8)
** 	GG (a, b, c, d, M9, 5, 0x21e1cde6)
** 	GG (d, a, b, c, M14, 9, 0xc33707d6)
** 	GG (c, d, a, b, M3, 14, 0xf4d50d87)
** 	GG (b, c, d, a, M8, 20, 0x455a14ed)
** 	GG (a, b, c, d, M13, 5, 0xa9e3e905)
** 	GG (d, a, b, c, M2, 9, 0xfcefa3f8)
** 	GG (c, d, a, b, M7, 14, 0x676f02d9)
** 	GG (b, c, d, a, M12, 20, 0x8d2a4c8a)
** 
** 	Round 3:
** 	HH (a, b, c, d, M5, 4, 0xfffa3942)
** 	HH (d, a, b, c, M8, 11, 0x8771f681)
** 	HH (c, d, a, b, M11, 16, 0x6d9d6122)
** 	HH (b, c, d, a, M14, 23, 0xfde5380c)
** 	HH (a, b, c, d, M1, 4, 0xa4beea44)
** 	HH (d, a, b, c, M4, 11, 0x4bdecfa9)
** 	HH (c, d, a, b, M7, 16, 0xf6bb4b60)
** 	HH (b, c, d, a, M10, 23, 0xbebfbc70)
** 	HH (a, b, c, d, M13, 4, 0x289b7ec6)
** 	HH (d, a, b, c, M0, 11, 0xeaa127fa)
** 	HH (c, d, a, b, M3, 16, 0xd4ef3085)
** 	HH (b, c, d, a, M6, 23, 0x04881d05)
** 	HH (a, b, c, d, M9, 4, 0xd9d4d039)
** 	HH (d, a, b, c, M12, 11, 0xe6db99e5)
** 	HH (c, d, a, b, M15, 16, 0x1fa27cf8)
** 	HH (b, c, d, a, M2, 23, 0xc4ac5665)
** 
** 	Round 4:
** 	II (a, b, c, d, M0, 6, 0xf4292244)
** 	II (d, a, b, c, M7, 10, 0x432aff97)
** 	II (c, d, a, b, M14, 15, 0xab9423a7)
** 	II (b, c, d, a, M5, 21, 0xfc93a039)
** 	II (a, b, c, d, M12, 6, 0x655b59c3)
** 	II (d, a, b, c, M3, 10, 0x8f0ccc92)
** 	II (c, d, a, b, M10, 15, 0xffeff47d)
** 	II (b, c, d, a, M1, 21, 0x85845dd1)
** 	II (a, b, c, d, M8, 6, 0x6fa87e4f)
** 	II (d, a, b, c, M15, 10, 0xfe2ce6e0)
** 	II (c, d, a, b, M6, 15, 0xa3014314)
** 	II (b, c, d, a, M13, 21, 0x4e0811a1)
** 	II (a, b, c, d, M4, 6, 0xf7537e82)
** 	II (d, a, b, c, M11, 10, 0xbd3af235)
** 	II (c, d, a, b, M2, 15, 0x2ad7d2bb)
** 	II (b, c, d, a, M9, 21, 0xeb86d391)
** 
** 	Those constants, ti, were chosen as follows:
** 
** 	In step i, ti is the integer part of 232*abs(sin(i)), where i is in radians.
** 	After all of this, a, b, c, and d are added to A, B, C, D, respectively, and the
** 	algorithm continues with the next block of data. The final output is the
** 	concatenation of A, B, C, and D.
*/

void	to_bin(t_fmt *fmt)
{
	fmt->out = ft_uintmaxtoa_base(*(size_t*)fmt->type.void_ptr, 2, "01");
	fmt->len.out = LEN(fmt->out, 0);
	ftprintf_format_nbr(fmt);
}

size_t	get_md5_padding(size_t len)
{
	size_t a;

	a = 0;
	while (a * 512 < (len + 64 + 1))
		a += 1;
	return (a * 512 - 64 - 1 - len);
}
char	*pad_pre_image(char *pre_image, size_t *len)
{
	size_t		orig_len;
	size_t		padding_len;
	char		*padded_pre_image;
	t_cnv		f;

	orig_len = LEN(pre_image, 0);
	padding_len = get_md5_padding(orig_len);
	*len = orig_len + 1 + padding_len + 64;
	if (!(padded_pre_image = ft_strnew(*len)))
		ft_ssl_err("error");
	ft_memcpy(padded_pre_image, pre_image, orig_len);
	padded_pre_image[orig_len] = '1';
	ft_czero(padded_pre_image + orig_len + 1, '0', padding_len);
	f = to_bin;
	ftprintf_register_cnv('b', &f);
	if (!ft_snprintf(padded_pre_image + orig_len + padding_len, 65, "%0.64b", (void*)&orig_len))
		return (NULL);
	return (padded_pre_image);
}

# define A 0
# define B 1
# define C 2
# define D 3

# define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
# define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
# define H(x, y, z) ((x) ^ (y) ^ (z))
# define I(x, y, z) ((y) ^ ((x) | (~z)))

# define GG(a, b, c, d, Mj, s, ti) (a = b + ((a + G(b,c,d ) + Mj+ ti) <<< s))
# define HH(a, b, c, d, Mj, s, ti) (a = b + ((a + H(b,c,d) + Mj+ ti) <<< s))
# define II(a, b, c, d, Mj, s, ti) (a = b + ((a + I(b,c,d ) + Mj+ ti) <<< s))

/* ROTATE_LEFT rotates x left n bits.
 */
#define ROTATE_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/*
** FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
** Rotation is separate from addition to prevent recomputation.
*/

// # define FF(a, b, c, d, Mj, s, ti) (a = b + ((a + F(b,c,d ) + Mj+ ti) <<< s))
// #define FF(a, b, c, d, x, s, ac) ((a) += (b)
// #define FF(a, b, c, d, x, s, ac) { \
//  (a) += F ((b), (c), (d)) + (x) + (UINT4)(ac); \
//  (a) = ROTATE_LEFT ((a), (s)); \
//   }
// #define GG(a, b, c, d, x, s, ac) { \
//  (a) += G ((b), (c), (d)) + (x) + (UINT4)(ac); \
//  (a) = ROTATE_LEFT ((a), (s)); \
//  (a) += (b); \
//   }
// #define HH(a, b, c, d, x, s, ac) { \
//  (a) += H ((b), (c), (d)) + (x) + (UINT4)(ac); \
//  (a) = ROTATE_LEFT ((a), (s)); \
//  (a) += (b); \
//   }
// #define II(a, b, c, d, x, s, ac) { \
//  (a) += I ((b), (c), (d)) + (x) + (UINT4)(ac); \
//  (a) = ROTATE_LEFT ((a), (s)); \
//  (a) += (b); \
//   }

// define (• is XOR,¥ is AND, ¦ is OR, and ¬ is NOT.)
char	*md5_transform(t_digest *digest)
{
	size_t		len;
	size_t		position;
	char		*padded_pre_image;
	uint32_t	chaining_vars[4];
	uint32_t	block_a;
	uint32_t	block_b;
	uint32_t	block_c;
	uint32_t	block_d;
	size_t		round;

	chaining_vars[A] = 0x01234567;
	chaining_vars[B] = 0x89abcdef;
	chaining_vars[C] = 0xfedcba98;
	chaining_vars[D] = 0x76543210;
	padded_pre_image = pad_pre_image(digest->pre_image, &len);
	// main_loop -> while 512-bit blocks remaining
	position = 0;
	while (position < len)
	{
		a = chaining_vars[A];
		b = chaining_vars[B];
		c = chaining_vars[C];
		d = chaining_vars[D];
		while ()
	}
	// If Mj represents the j th sub-block of the message (from 0 to 15), and <<<s
	// represents a left circular shift of s bits, the four operations are:
	// 
	// 
	// 	four rounds:
	// 		FF, GG, HH, II
	// 		each takes (a, b, c, d, M_x, 7|12|17|22, chaining_variable?)
	// 			the order of a, b, c, d is rotated once per application
	// 				can use modulo to check order
	// 			x  increases by 1 per application
	// 				use union?
	// 			what to call 7|112|17|22, can we tie them to param order modulo?
	// 			t_i is radian_lookup_table[i], can we tie them to x?
	// 		
	// 	The four rounds (64 steps) look like:
	// 	Round 1:
	// 	FF (a, b, c, d, M0, 7, 0xd76aa478)
	// 	FF (d, a, b, c, M1, 12, 0xe8c7b756)
	// 	FF (c, d, a, b, M2, 17, 0x242070db)
	// 	FF (b, c, d, a, M3, 22, 0xc1bdceee)
	// 	FF (a, b, c, d, M4, 7, 0xf57c0faf)
	// 	FF (d, a, b, c, M5, 12, 0x4787c62a)
	// 	FF (c, d, a, b, M6, 17, 0xa8304613)
	// 	FF (b, c, d, a, M7, 22, 0xfd469501)
	// 	FF (a, b, c, d, M8, 7, 0x698098d8)
	// 	FF (d, a, b, c, M9, 12, 0x8b44f7af)
	// 	FF (c, d, a, b, M10, 17, 0xffff5bb1)
	// 	FF (b, c, d, a, M11, 22, 0x895cd7be)
	// 	FF (a, b, c, d, M12, 7, 0x6b901122)
	// 	FF (d, a, b, c, M13, 12, 0xfd987193)
	// 	FF (c, d, a, b, M14, 17, 0xa679438e)
	// 	FF (b, c, d, a, M15, 22, 0x49b40821)
	// 
	// 	Round 2:
	// 	GG (a, b, c, d, M1, 5, 0xf61e2562)
	// 	GG (d, a, b, c, M6, 9, 0xc040b340)
	// 	GG (c, d, a, b, M11, 14, 0x265e5a51)
	// 	GG (b, c, d, a, M0, 20, 0xe9b6c7aa)
	// 	GG (a, b, c, d, M5, 5, 0xd62f105d)
	// 	GG (d, a, b, c, M10, 9, 0x02441453)
	// 	GG (c, d, a, b, M15, 14, 0xd8a1e681)
	// 	GG (b, c, d, a, M4, 20, 0xe7d3fbc8)
	// 	GG (a, b, c, d, M9, 5, 0x21e1cde6)
	// 	GG (d, a, b, c, M14, 9, 0xc33707d6)
	// 	GG (c, d, a, b, M3, 14, 0xf4d50d87)
	// 	GG (b, c, d, a, M8, 20, 0x455a14ed)
	// 	GG (a, b, c, d, M13, 5, 0xa9e3e905)
	// 	GG (d, a, b, c, M2, 9, 0xfcefa3f8)
	// 	GG (c, d, a, b, M7, 14, 0x676f02d9)
	// 	GG (b, c, d, a, M12, 20, 0x8d2a4c8a)
	// 
	// 	Round 3:
	// 	HH (a, b, c, d, M5, 4, 0xfffa3942)
	// 	HH (d, a, b, c, M8, 11, 0x8771f681)
	// 	HH (c, d, a, b, M11, 16, 0x6d9d6122)
	// 	HH (b, c, d, a, M14, 23, 0xfde5380c)
	// 	HH (a, b, c, d, M1, 4, 0xa4beea44)
	// 	HH (d, a, b, c, M4, 11, 0x4bdecfa9)
	// 	HH (c, d, a, b, M7, 16, 0xf6bb4b60)
	// 	HH (b, c, d, a, M10, 23, 0xbebfbc70)
	// 	HH (a, b, c, d, M13, 4, 0x289b7ec6)
	// 	HH (d, a, b, c, M0, 11, 0xeaa127fa)
	// 	HH (c, d, a, b, M3, 16, 0xd4ef3085)
	// 	HH (b, c, d, a, M6, 23, 0x04881d05)
	// 	HH (a, b, c, d, M9, 4, 0xd9d4d039)
	// 	HH (d, a, b, c, M12, 11, 0xe6db99e5)
	// 	HH (c, d, a, b, M15, 16, 0x1fa27cf8)
	// 	HH (b, c, d, a, M2, 23, 0xc4ac5665)
	// 
	// 	Round 4:
	// 	II (a, b, c, d, M0, 6, 0xf4292244)
	// 	II (d, a, b, c, M7, 10, 0x432aff97)
	// 	II (c, d, a, b, M14, 15, 0xab9423a7)
	// 	II (b, c, d, a, M5, 21, 0xfc93a039)
	// 	II (a, b, c, d, M12, 6, 0x655b59c3)
	// 	II (d, a, b, c, M3, 10, 0x8f0ccc92)
	// 	II (c, d, a, b, M10, 15, 0xffeff47d)
	// 	II (b, c, d, a, M1, 21, 0x85845dd1)
	// 	II (a, b, c, d, M8, 6, 0x6fa87e4f)
	// 	II (d, a, b, c, M15, 10, 0xfe2ce6e0)
	// 	II (c, d, a, b, M6, 15, 0xa3014314)
	// 	II (b, c, d, a, M13, 21, 0x4e0811a1)
	// 	II (a, b, c, d, M4, 6, 0xf7537e82)
	// 	II (d, a, b, c, M11, 10, 0xbd3af235)
	// 	II (c, d, a, b, M2, 15, 0x2ad7d2bb)
	// 	II (b, c, d, a, M9, 21, 0xeb86d391)
	// 
	// 	Those constants, ti, were chosen as follows:
	// 
	// 	In step i, ti is the integer part of 232*abs(sin(i)), where i is in radians.
	// 	After all of this, a, b, c, and d are added to A, B, C, D, respectively, and the
	// 	algorithm continues with the next block of data. The final output is the
	// 	concatenation of A, B, C, and D.
	ft_printf("%s", padded_pre_image);
	return (NULL);
}

void	md5(void *input)
{
	size_t		i;
	size_t		total;
	t_digest	*digest;
	char		*hash;
	t_md5_state	*state;

	i = 0;
	state = (t_md5_state*)input;
	total = state->digests->current;
	while (i <= total - sizeof(t_digest))
	{
		digest = (t_digest*)&((char*)state->digests->buf)[i];
		hash = md5_transform(digest);
		ft_memcpy(digest->hash_value, hash, MD5_HASH_SIZE);
		i += sizeof(t_digest);
	}
	print_md5_state(state);
}
