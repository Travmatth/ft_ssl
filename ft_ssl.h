/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 12:34:43 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/10 12:00:34 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include <fcntl.h>
# include "libftprintf/srcs/includes/ft_printf.h"


// to libftprintf
# define DIGEST_SZ sizeof(t_digest)
# define LESSER(x, y) (x < y ? x : y)
# define STDIN 0
# define STDOUT 1
# define STDERR 2

# define FROM_FILE 0
# define FROM_STRING 1
# define FROM_STDIN 2
# define FT_SSL_FUNCS 1
# define FT_MD5_NO_SUCH_FILE "md5: no such file"

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

typedef void	(*t_f)(t_state *state);

typedef struct	s_ptrs
{
	char		*cmd;
	t_f			f;
}				t_ptrs;

typedef struct	s_digest
{
	char		*msg;
	char		type;
}				t_digest;

typedef struct	s_state
{
	char		flags[BITNSLOTS(5)];
	t_buf		*digests;
}				t_state;

typedef struct	s_hash
{
	t_buf		*out;
	t_state	*in;
}				t_hash;

void			ft_ssl_err(const char *message) __attribute__((noreturn));
void			md5(t_state *state);
t_state			*parse_opts(int argc, char **argv);
#endif
