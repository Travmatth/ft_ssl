/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 12:34:43 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/20 14:25:35 by tmatthew         ###   ########.fr       */
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

void				md5(void *input);
void				*parse_md5_opts(int argc, char **argv);
void				print_md5_state(t_md5_state *state);
void				ft_ssl_err(const char *message) __attribute__((noreturn));
#endif
