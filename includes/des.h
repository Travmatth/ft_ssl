/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/13 11:06:22 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/15 22:18:25 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DES_H
# define DES_H

# include "ft_ssl.h"

# define SET_DECRYPT(v) (BITSET(v, 0))
# define SET_ENCRYPT(v) (BITSET(v, 1))
# define SET_INPUT(v) (BITSET(v, 2))
# define SET_A(v) (BITSET(v, 3))
# define SET_K(v) (BITSET(v, 4))

# define GET_DECRYPT(v) (BITTEST(v, 0))
# define GET_ENCRYPT(v) (BITTEST(v, 1))
# define GET_INPUT(v) (BITTEST(v, 2))
# define GET_A(v) (BITTEST(v, 3))
# define GET_K(v) (BITTEST(v, 4))

# define ROT_28B_L(x, y) ((((x | (x >> 28)) << y) >> 36) << 36)

typedef struct		s_desctx
{
	uint8_t			*key;
	uint8_t			*salt;
	uint8_t			*in_text;
	uint8_t			*out_text;
	uint8_t			*password;
	uint64_t		init_vector;
	size_t			k_len;
	size_t			s_len;
	size_t			i_len;
	size_t			o_len;
	uint8_t			p_len;
	size_t			pass_len;
	void			(*pre_permute_chaining)(struct s_desctx *ctx
											, uint64_t *block
											, uint64_t *permuted_block
											, uint64_t *iv);
	void			(*post_permute_chaining)(struct s_desctx *ctx
											, uint64_t *block
											, uint64_t *permuted_block
											, uint64_t *iv);
	int				out_file;
	unsigned char	flags[BITNSLOTS(4)];
}					t_desctx;

void				des_wrapper_print(t_desctx *ctx);
void				configure_des_params(t_desctx *ctx);
uint8_t				*create_des_key(t_desctx *ctx);
uint64_t			permute_block(uint8_t *map, uint64_t block, size_t limit);
uint64_t			feistel_cipher(uint64_t	block, uint64_t key);
uint64_t			des_permute(uint64_t block
							, uint64_t keyschedule[16]
							, int encipher);
void				key_operation_mode(int decrypt, uint64_t keyschedule[16]);
void				des_cbc_pre_permute_hook(t_desctx *ctx
											, uint64_t *block
											, uint64_t *permuted_block
											, uint64_t *iv);
void				des_cbc_post_permute_hook(t_desctx *ctx
											, uint64_t *block
											, uint64_t *permuted_block
											, uint64_t *iv);
void				des_null_permute_hook(t_desctx *ctx
										, uint64_t *block
										, uint64_t *permuted_block
										, uint64_t *iv);
void				*parse_des_opts(int argc, char **argv);
void				des_init(t_desctx *ctx, uint64_t keyschedule[16]);
void				des_update(t_desctx *ctx
							, uint8_t *in_text
							, uint64_t keyschedule[16]);
void				des_final(t_desctx *ctx
							, uint8_t *in_text
							, uint64_t keyschedule[16]
							, size_t i_len);
void				des_wrapper(void *input);
#endif
