/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 12:34:43 by tmatthew          #+#    #+#             */
/*   Updated: 2018/08/13 19:20:04 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include <fcntl.h>
# include "../libftprintf/srcs/includes/ft_printf.h"
# include "hashing.h"
# include "base64.h"
# include "des.h"

# define FROM_FILE 0
# define FROM_STRING 1
# define FROM_STDIN 2
# define NO_INPUT 3

# define FT_SSL_FUNCS 4

# define FROM_BITS(x) (x / 8)
# define TO_BITS(x) (x * 8)

/*
** ROTATE_LEFT rotates a 32 bit x left n bits.
*/

# define R_LEFT(x, n) (((x) << (n)) | ((x) >> (32-(n))))

/*
** ROTATE_LEFT rotates a 32 bit x left n bits.
*/

# define R_RIGHT(x, n) (((x) >> (n)) | ((x) << (32-(n))))

typedef void	(*t_f)(void *state);
typedef void	*(*t_p)(int argc, char **argv);

typedef struct	s_ptrs
{
	char		*cmd;
	t_f			f;
	t_p			p;
}				t_ptrs;

void			*read_from_file(t_hash_state *state
							, t_digest *digest, char **argv, int *i);
void			read_from_stdin(t_hash_state *state, t_digest *digest);
void			ft_ssl_err(const char *message) __attribute__((noreturn));
#endif
