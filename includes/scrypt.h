/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scrypt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 19:21:44 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/26 17:44:35 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCRYPT_H
# define SCRYPT_H

# include "ft_ssl.h"

/*
** xor_word(len, &block_tmp[(cost_tmp[len - 16] & (opts->cost_param - 1)) * len]
**			, cost_tmp);
** xor_word(len, &block_tmp[(internal_tmp[len - 16] &
**			(opts->cost_param - 1)) * len], internal_tmp);
*/

# define BL_OFFSET(t, len, o) ((t[len - 16] & (o->cost_param - 1)) * len)

typedef struct	s_scrypt
{
	unsigned	cost_param;
	unsigned	block_size;
	unsigned	parallel_param;
	uint8_t		*key;
	uint8_t		*salt;
	uint8_t		*password;
	size_t		k_len;
	size_t		s_len;
	uint8_t		p_len;
}				t_scrypt;

typedef struct s_desctx	t_desctx;
void			verify_scrypt_params(unsigned parallel_param
							, unsigned block_size
							, unsigned cost_param);
void			xor_word(uint32_t len, uint32_t *src, uint32_t *dst);
void			to_little_endian(unsigned char *blocks
								, uint32_t *cost_tmp
								, uint32_t len);
void			to_big_endian(unsigned char *blocks
							, uint32_t *cost_tmp
							, uint32_t len);
void			scrypt(t_desctx *ctx, t_scrypt *opts);
#endif
