/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 12:34:43 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/25 20:35:26 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include <fcntl.h>
# include "libftprintf/srcs/includes/ft_printf.h"

# define DIGEST_SZ sizeof(t_digest)
# define FROM_FILE 0
# define FROM_STRING 1
# define FROM_STDIN 2
# define NO_INPUT 3
# define FT_SSL_FUNCS 2
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
# define E 4
# define F 5
# define G 6
# define H 7

/*
** ROTATE_LEFT rotates a 32 bit x left n bits.
*/

# define R_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/*
** ROTATE_LEFT rotates a 32 bit x left n bits.
*/

# define R_RIGHT(x, n) (((x) >> (n)) | ((x) << (32-(n))))

typedef struct	s_digest
{
	char		*pre_image;
	char		*file_name;
	int			type;
	char		*hash_value;
}				t_digest;

typedef struct	s_hash_state
{
	char		flags[BITNSLOTS(5)];
	t_buf		*digests;
}				t_hash_state;

typedef void	(*t_f)(void *state);
typedef void	*(*t_p)(int argc, char **argv);

typedef struct	s_ptrs
{
	char		*cmd;
	t_f			f;
	t_p			p;
}				t_ptrs;

char			*from_hex_hash(char *output
							, unsigned char *hash_value, size_t len);
void			print_hash_state(char *hash, t_hash_state *state);
void			*read_from_file(t_hash_state *state
							, t_digest *digest, char **argv, int *i);
void			read_from_stdin(t_hash_state *state, t_digest *digest);
unsigned char	*pad_pre_image(char *pre_image, size_t *len);
size_t			get_hash_padding(size_t len);
void			*parse_hash_opts(int argc, char **argv);
void			ft_ssl_err(const char *message) __attribute__((noreturn));
void			md5(void *input);
void			sha256(void *input);
#endif
