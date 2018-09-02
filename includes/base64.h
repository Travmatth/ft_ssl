/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/08 21:10:12 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/01 13:02:32 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASE64_H
# define BASE64_H

# include "ft_ssl.h"

# define SET_D(v) (BITSET(v, 0))
# define SET_E(v) (BITSET(v, 1))
# define SET_I(v) (BITSET(v, 2))
# define SET_O(v) (BITSET(v, 3))

# define GET_D(v) (BITTEST(v, 0))
# define GET_E(v) (BITTEST(v, 1))
# define GET_I(v) (BITTEST(v, 2))
# define GET_O(v) (BITTEST(v, 3))

/*
** table to look up base64 equivalent chars
*/

# define CHRS "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

typedef struct	s_base64
{
	char		mode[BITNSLOTS(1)];
	char		*message;
	char		*input;
	char		*output;
}				t_base64;

void			*parse_base64_opts(int argc, char **argv);
char			*base64_decode(char *message, size_t len);
char			*base64_encode(char *message, size_t base_len);
void			base64(void *input);
#endif
