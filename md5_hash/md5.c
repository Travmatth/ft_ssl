/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/30 20:13:01 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/11 19:02:51 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ssl.h"

/*
** pseudo
** pad pre_image
** add '1'
** add 0's such that len((pre_image + padding) % 64 == 0)
** append 64 bit representation of msg len (b4 padding) 
** (2 prev steps make (x * sz == 512))
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

char	*pad_pre_image(char *pre_image, size_t *len)
{
	size_t		orig_len;
	size_t		padding_len;
	char		*padded_pre_image;
	uint64_t	out;

	orig_len = LEN(pre_image, 0);
	padding_len = (orig_len + 1) % 448; 
	*len = orig_len + 1 + padding_len + sizeof(uint64_t);
	if (!(padded_pre_image = ft_memalloc(*len)))
		return ;
	ft_memcpy(padded_pre_image, pre_image, len);
	padded_pre_image[orig_len + 1] = '1';
	ft_bzero(padded_pre_image + orig_len + 1, padding_len);
	out = (uint64_t)len;
	ft_memcpy(padded_pre_image + orig_len + 1 + padding_len, (void*)&out, sizeof(uint64_t));
	return (padded_pre_image);
}

char	*make_md5_hash(t_digest *digest)
{
	size_t	len;
	char	*padded_pre_image;

	padded_pre_image = pad_pre_image(digest->pre_image, &len);
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
		hash = make_md5_hash(digest);
		ft_memcpy(digest->hash_value, hash, MD5_HASH_SIZE);
		i += sizeof(t_digest);
	}
	print_md5_state(state);
}
