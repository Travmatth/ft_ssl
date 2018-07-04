/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 12:34:43 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/03 20:21:08 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include "libftprintf/srcs/includes/ft_printf.h"

# define STDIN 1
# define STRING 0
# define FT_SSL_FUNCS 1
# define SET_P(v) (BITSET(v, 0))
# define SET_Q(v) (BITSET(v, 1))
# define SET_R(v) (BITSET(v, 2))
# define SET_S(v) (BITSET(v, 3))
# define GET_P(v) (BITTEST(v, 0))
# define GET_Q(v) (BITTEST(v, 1))
# define GET_R(v) (BITTEST(v, 2))
# define GET_S(v) (BITTEST(v, 3))

typedef void	(*t_f)(int *argc, char **argv);

typedef struct	s_ptrs
{
	char		*cmd;
	t_f			f;
}				t_ptrs;

typedef struct	s_msg
{
	t_buf		*msg;
	char		type;
}				t_msg;

typedef struct	s_digest
{
	char		flags[BITNSLOTS(4)];
	t_msg		**digests;
}				t_digest;

typedef struct	s_hash
{
	t_buf		*out;
	t_digest	*in;
}				t_hash;

void			ft_ssl_err(const char *message) __attribute__((noreturn));
void			md5(int *argc, char **argv);
#endif
