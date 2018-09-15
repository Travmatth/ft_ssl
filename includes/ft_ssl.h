/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ssl.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/16 12:34:43 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/14 19:27:34 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_SSL_H
# define FT_SSL_H

# include <fcntl.h>
# include "../libftprintf/srcs/includes/ft_printf.h"
# include "sha256.h"
# include "hashing.h"
# include "base64.h"
# include "pbkdf2.h"
# include "des.h"
# include "salsa.h"
# include "scrypt.h"

# define FROM_FILE 0
# define FROM_STRING 1
# define FROM_STDIN 2
# define NO_INPUT 3

# define FT_SSL_FUNCS 6
# define SHA2_256_DLEN 32

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

typedef char	*(*t_h)(char *output, char *pre_image);

typedef struct	s_ptrs
{
	char		*cmd;
	t_f			f;
	t_p			p;
}				t_ptrs;

void			*read_from_file(t_hash_state *state
							, t_digest *digest, char **argv, int *i);
void			ft_ssl_cmd_err(char *command);
void			read_from_stdin(t_hash_state *state, t_digest *digest);
void			ft_ssl_err(const char *message) __attribute__((noreturn));
#endif
