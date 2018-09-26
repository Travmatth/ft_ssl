/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/13 11:06:22 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/25 19:15:21 by tmatthew         ###   ########.fr       */
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
# define SET_V(v) (BITSET(v, 5))
# define SET_NEED_V(v) (BITSET(v, 6))

# define GET_DECRYPT(v) (BITTEST(v, 0))
# define GET_ENCRYPT(v) (BITTEST(v, 1))
# define GET_INPUT(v) (BITTEST(v, 2))
# define GET_A(v) (BITTEST(v, 3))
# define GET_K(v) (BITTEST(v, 4))
# define GET_V(v) (BITTEST(v, 5))
# define GET_NEED_V(v) (BITTEST(v, 6))

# define ROT_28B_L(x, y) ((((x | (x >> 28)) << y) >> 36) << 36)

typedef struct		s_desopt
{
	char			*name;
	void			(*pre_permute_chaining)(struct s_desctx *ctx
											, uint64_t *block
											, uint64_t *permuted_block
											, uint64_t *iv);
	void			(*post_permute_chaining)(struct s_desctx *ctx
											, uint64_t *block
											, uint64_t *permuted_block
											, uint64_t *iv);
	uint8_t			needs_v;
}					t_desopt;

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
	void			(*pre_permute_chaining)(struct s_desctx *ctx
											, uint64_t *block
											, uint64_t *permuted_block
											, uint64_t *iv);
	void			(*post_permute_chaining)(struct s_desctx *ctx
											, uint64_t *block
											, uint64_t *permuted_block
											, uint64_t *iv);
	int				out_file;
	unsigned char	flags[BITNSLOTS(7)];
}					t_desctx;

/*
** des helpers
*/

int					parse_des_mode(t_desctx *ctx, char *command);
void				verify_des_params(t_desctx *ctx);
void				process_init_vector(t_desctx *ctx);
void				des_wrapper_print(t_desctx *ctx);
void				configure_des_params(t_desctx *ctx, const char *mode);
void				create_des_key(t_desctx *ctx);
void				*parse_des_opts(int argc, char **argv);
void				des_decode_trim_padding(t_desctx *ctx);

/*
** des algo
*/

uint64_t			permute_block(uint8_t *map, uint64_t block, size_t limit);
uint64_t			feistel_cipher(uint64_t	block, uint64_t key);
uint64_t			des_permute(uint64_t block
							, uint64_t keyschedule[16]
							, int encipher);

/*
** des
*/

void				des_init(uint64_t keyschedule[16], uint8_t *hex_key);
void				des_update(t_desctx *ctx
							, uint8_t *in_text
							, uint64_t keyschedule[16]);
void				des_final(t_desctx *ctx
							, uint8_t *in_text
							, uint64_t keyschedule[16]
							, size_t i_len);
void				des_wrapper(void *input);

/*
** des3 algo
*/

void				des3_init(t_desctx *ctx
							, uint64_t **keyschedule
							, uint8_t *hex_key);
void				des3_update(t_desctx *ctx
								, uint8_t *in_text
								, uint64_t *keyschedule[3]);
void				des3_final(t_desctx *ctx
								, uint8_t *in_text
								, uint64_t *keyschedule[3]
								, size_t i_len);
void				des3_wrapper(void *input);

/*
** des modes
*/

void				des_null_permute_hook(t_desctx *ctx
										, uint64_t *block
										, uint64_t *permuted_block
										, uint64_t *iv);
void				des_cbc_pre_permute_hook(t_desctx *ctx
											, uint64_t *block
											, uint64_t *permuted_block
											, uint64_t *iv);
void				des_cbc_post_permute_hook(t_desctx *ctx
											, uint64_t *block
											, uint64_t *permuted_block
											, uint64_t *iv);
void				des_pcbc_pre_permute_hook(t_desctx *ctx
										, uint64_t *block
										, uint64_t *permuted_block
										, uint64_t *iv);
void				des_pcbc_post_permute_hook(t_desctx *ctx
										, uint64_t *block
										, uint64_t *permuted_block
										, uint64_t *iv);
void				des_cfb_pre_permute_hook(t_desctx *ctx
										, uint64_t *block
										, uint64_t *permuted_block
										, uint64_t *iv);
void				des_cfb_post_permute_hook(t_desctx *ctx
										, uint64_t *block
										, uint64_t *permuted_block
										, uint64_t *iv);
void				des_ofb_pre_permute_hook(t_desctx *ctx
										, uint64_t *block
										, uint64_t *permuted_block
										, uint64_t *iv);
void				des_ofb_post_permute_hook(t_desctx *ctx
										, uint64_t *block
										, uint64_t *permuted_block
										, uint64_t *iv);
void				des_ctr_pre_permute_hook(t_desctx *ctx
										, uint64_t *block
										, uint64_t *permuted_block
										, uint64_t *iv);
void				des_ctr_post_permute_hook(t_desctx *ctx
										, uint64_t *block
										, uint64_t *permuted_block
										, uint64_t *iv);
#endif
