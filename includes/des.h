/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/13 11:06:22 by tmatthew          #+#    #+#             */
/*   Updated: 2018/08/31 20:31:14 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DES_H
# define DES_H

# include "ft_ssl.h"

typedef struct		s_desctx
{
	uint8_t			*key;
	uint8_t			*salt;
	uint8_t			*plaintext;
	uint8_t			*ciphertext;
	uint8_t			*password;
	uint8_t			*init_vector;
	size_t			klen;
	size_t			slen;
	size_t			plen;
	size_t			clen;
	size_t			passlen;
	size_t			ivlen;
	void			(*pre_permute_chaining)(t_desctx *ctx, uint64_t *block, uint8_t *plaintext, uint64_t keyschedule[16]);
	void			(*post_permute_chaining)(t_desctx *ctx, uint64_t *block, uint8_t *plaintext, uint64_t keyschedule[16]);
	int				out_file;
	unsigned char	flags[BITNSLOTS(4)];
}					t_desctx;

# define SET_DECRYPT(v) (BITSET(v, 0))
# define SET_ENCRYPT(v) (BITSET(v, 1))
# define SET_INPUT(v) (BITSET(v, 2))
# define SET_A(v) (BITSET(v, 3))

# define GET_DECRYPT(v) (BITTEST(v, 0))
# define GET_ENCRYPT(v) (BITTEST(v, 1))
# define GET_INPUT(v) (BITTEST(v, 2))
# define GET_A(v) (BITTEST(v, 3))

void			*parse_des_opts(int argc, char **argv);
void			des(void *input);
#endif
