/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 12:34:43 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/21 13:40:55 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include <fcntl.h>
# include "libftprintf/srcs/includes/ft_printf.h"


// to libftprintf
# define LESSER(x, y) (x < y ? x : y)
# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define DIGEST_SZ sizeof(t_digest)
# define FROM_FILE 0
# define FROM_STRING 1
# define FROM_STDIN 2
# define NO_INPUT 3
# define FT_SSL_FUNCS 1
# define MD5_HASH_SIZE 33
# define FT_MD5_NO_SUCH_FILE "ft_ssl: %s: no such file or directory\n"
# define MD5_DIGEST(state, i) ((t_digest*)((char*)state->digests->buf + i))

# define SET_P(v) (BITSET(v, 0))
# define SET_Q(v) (BITSET(v, 1))
# define SET_R(v) (BITSET(v, 2))
# define SET_S(v) (BITSET(v, 3))
# define SET_F(v) (BITSET(v, 4))

# define GET_P(v) (BITTEST(v, 0))
# define GET_Q(v) (BITTEST(v, 1))
# define GET_R(v) (BITTEST(v, 2))
# define GET_S(v) (BITTEST(v, 3))
# define GET_F(v) (BITTEST(v, 4))

# define FROM_BITS(x) (x / 8)
# define TO_BITS(x) (x * 8)
# define A 0
# define B 1
# define C 2
# define D 3

# define F(x, y, z) (((x) & (y)) | ((~x) & (z)))
# define G(x, y, z) (((x) & (z)) | ((y) & (~z)))
# define H(x, y, z) ((x) ^ (y) ^ (z))
# define I(x, y, z) ((y) ^ ((x) | (~z)))

/*
** ROTATE_LEFT rotates x left n bits.
*/

# define R_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))
# define MD5_SUM() ()


/*
** FF, GG, HH, and II transformations for rounds 1, 2, 3, and 4.
** Rotation is separate from addition to prevent recomputation.
*/


# define FF(a, b, c, d, m_j, s, t_i) (a = b + R_LEFT((a + F(b, c, d) + m_j + t_i), s))
# define GG(a, b, c, d, m_j, s, t_i) (a = b + R_LEFT((a + G(b, c, d) + m_j + t_i), s))
# define HH(a, b, c, d, m_j, s, t_i) (a = b + R_LEFT((a + H(b, c, d) + m_j + t_i), s))
# define II(a, b, c, d, m_j, s, t_i) (a = b + R_LEFT((a + I(b, c, d) + m_j + t_i), s))

typedef uint8_t		t_byte;
typedef uint32_t	t_word;

typedef struct		s_digest
{
	char			*pre_image;
	char			*file_name;
	int				type;
	char			*hash_value;
}					t_digest;

typedef struct		s_md5_state
{
	char			flags[BITNSLOTS(5)];
	t_buf			*digests;
}					t_md5_state;

typedef void		(*t_f)(void *state);
typedef void		*(*t_p)(int argc, char **argv);

typedef struct		s_ptrs
{
	char			*cmd;
	t_f				f;
	t_p				p;
}					t_ptrs;

typedef struct	s_md5_round
{
	uint32_t	(*f)(a, b, c);
	uint32_t	signed_constant;
	short		sub_block;
	short		step;
}				t_md5_round;

t_md5_rounds[4][1] = {
	{
		{f, 0, 7, 0xd76aa478},
		{f, 1, 12, 0xe8c7b756},
		{f, 2, 17, 0x242070db},
		{f, 3, 22, 0xc1bdceee},
		{f, 4, 7, 0xf57c0faf},
		{f, 5, 12, 0x4787c62a},
		{f, 6, 17, 0xa8304613},
		{f, 7, 22, 0xfd469501},
		{f, 8, 7, 0x698098d8},
		{f, 9, 12, 0x8b44f7af},
		{f, 10, 17, 0xffff5bb1},
		{f, 11, 22, 0x895cd7be},
		{f, 12, 7, 0x6b901122},
		{f, 13, 12, 0xfd987193},
		{f, 14, 17, 0xa679438e},
		{f, 15, 22, 0x49b40821}
	},
	{
		{g, 1, 5, 0xf61e2562},
		{g, 6, 9, 0xc040b340},
		{g, 11, 14, 0x265e5a51},
		{g, 0, 20, 0xe9b6c7aa},
		{g, 5, 5, 0xd62f105d},
		{g, 10, 9, 0x02441453},
		{g, 15, 14, 0xd8a1e681},
		{g, 4, 20, 0xe7d3fbc8},
		{g, 9, 5, 0x21e1cde6},
		{g, 14, 9, 0xc33707d6},
		{g, 3, 14, 0xf4d50d87},
		{g, 8, 20, 0x455a14ed},
		{g, 13, 5, 0xa9e3e905},
		{g, 2, 9, 0xfcefa3f8},
		{g, 7, 14, 0x676f02d9},
		{g, 12, 20, 0x8d2a4c8a},
	},
	{
		{h, 5, 4, 0xfffa3942},
		{h, 8, 11, 0x8771f681},
		{h, 11, 16, 0x6d9d6122},
		{h, 14, 23, 0xfde5380c},
		{h, 1, 4, 0xa4beea44},
		{h, 4, 11, 0x4bdecfa9},
		{h, 7, 16, 0xf6bb4b60},
		{h, 10, 23, 0xbebfbc70},
		{h, 13, 4, 0x289b7ec6},
		{h, 0, 11, 0xeaa127fa},
		{h, 3, 16, 0xd4ef3085},
		{h, 6, 23, 0x04881d05},
		{h, 9, 4, 0xd9d4d039},
		{h, 12, 11, 0xe6db99e5},
		{h, 15, 16, 0x1fa27cf8},
		{h, 2, 23, 0xc4ac5665}
	},
	{
		{i, 0, 6, 0xf4292244},
		{i, 7, 10, 0x432aff97},
		{i, 14, 15, 0xab9423a7},
		{i, 5, 21, 0xfc93a039},
		{i, 12, 6, 0x655b59c3},
		{i, 3, 10, 0x8f0ccc92},
		{i, 10, 15, 0xffeff47d},
		{i, 1, 21, 0x85845dd1},
		{i, 8, 6, 0x6fa87e4f},
		{i, 15, 10, 0xfe2ce6e0},
		{i, 6, 15, 0xa3014314},
		{i, 13, 21, 0x4e0811a1},
		{i, 4, 6, 0xf7537e82},
		{i, 11, 10, 0xbd3af235},
		{i, 2, 15, 0x2ad7d2bb},
		{i, 9, 21, 0xeb86d391}
	}
};

void				md5(void *input);
void				*parse_md5_opts(int argc, char **argv);
void				print_md5_state(t_md5_state *state);
void				ft_ssl_err(const char *message) __attribute__((noreturn));
#endif
