/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/13 11:06:22 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/03 10:31:21 by tmatthew         ###   ########.fr       */
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
	void			(*pre_permute_chaining)(struct s_desctx *ctx, uint64_t *block, uint8_t *plaintext, uint64_t keyschedule[16]);
	void			(*post_permute_chaining)(struct s_desctx *ctx, uint64_t *block, uint8_t *plaintext, uint64_t keyschedule[16]);
	int				out_file;
	unsigned char	flags[BITNSLOTS(4)];
}					t_desctx;

typedef 
# define SET_DECRYPT(v) (BITSET(v, 0))
# define SET_ENCRYPT(v) (BITSET(v, 1))
# define SET_INPUT(v) (BITSET(v, 2))
# define SET_A(v) (BITSET(v, 3))
// # define SET_NP(v) (BITSET(v, 4))

# define GET_DECRYPT(v) (BITTEST(v, 0))
# define GET_ENCRYPT(v) (BITTEST(v, 1))
# define GET_INPUT(v) (BITTEST(v, 2))
# define GET_A(v) (BITTEST(v, 3))
// # define GET_NP(v) (BITTEST(v, 4))

uint8_t			*create_des_key(t_desctx *ctx);
uint64_t		permute_block(uint8_t *map, uint64_t block);
uint64_t		des_f(uint64_t	block, uint64_t key);
uint64_t		des_permute(t_desctx *ctx, uint64_t block, uint64_t keyschedule[16]);
void			key_operation_mode(int decrypt, uint64_t keyschedule[16]);
void			des_cbc_pre_permute_hook(t_desctx *ctx
						, uint64_t *block
						, uint8_t *plaintext
						, uint64_t keyschedule[16]);
void			des_cbc_post_permute_hook(t_desctx *ctx
						, uint64_t *block
						, uint8_t *plaintext
						, uint64_t keyschedule[16]);
void			des_null_permute_hook(t_desctx *ctx
						, uint64_t *block
						, uint8_t *plaintext
						, uint64_t keyschedule[16]);
void			*parse_des_opts(int argc, char **argv);
void			des_init(t_desctx *ctx, uint64_t keyschedule[16]);
void			des_update(t_desctx *ctx
						, uint8_t *plaintext
						, uint64_t keyschedule[16]);
void			des_final(t_desctx *ctx
						, uint8_t *plaintext
						, uint64_t keyschedule[16]
						, size_t plen);
void			des_wrapper(void *input);
#endif
