/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   base64.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/08 21:10:12 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/26 14:55:13 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BASE64_H
# define BASE64_H

# include "ft_ssl.h"

# define SB64_DECODE(v) (BITSET(v, 0))
# define SB64_ENCODE(v) (BITSET(v, 1))
# define SB64_IN(v) (BITSET(v, 2))
# define SB64_OUT(v) (BITSET(v, 3))

# define GB64_DECODE(v) (BITTEST(v, 0))
# define GB64_ENCODE(v) (BITTEST(v, 1))
# define GB64_IN(v) (BITTEST(v, 2))
# define GB64_OUT(v) (BITTEST(v, 3))

# define IS_LOWER(x) (x >= 'a' && x <= 'z')
# define IS_UPPER(x) (x >= 'A' && x <= 'Z')
# define IS_NUMBER(x) (x >= '0' && x <= '9')
# define IS_ALNUM(x) (IS_LOWER(x) || IS_UPPER(x) || IS_NUMBER(x))
# define IS_B64(x) (IS_ALNUM(x) || x == '+' || x == '/' || x == '=')

# define LEN_TO64(x) (((x * 4 + 1) / 3) + ((x % 3) ? (3 - (x % 3)) : 0))
# define LEN_FROM64(x) ((3 * x) / 4)

/*
** table to look up base64 equivalent chars
*/

# define B64 "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

typedef struct			s_b64
{
	unsigned char		*in;
	size_t				in_len;
	int					fd;
	char				mode[BITNSLOTS(4)];
}						t_b64;

typedef struct			s_base64
{
	unsigned char		*in;
	unsigned char		*out;
	size_t				in_len;
	size_t				out_len;
}						t_base64;

void					*b64_finalize(unsigned char *in, size_t *len);
void					*b64_normalize(void *final
									, char elem
									, size_t i
									, int *stop);
void					b64_encode(t_base64 *ctx
								, unsigned char *in
								, unsigned char *out);
void					b64_decode(t_base64 *ctx
								, unsigned char *in
								, unsigned char *out);
void					*parse_b64_opts(int argc, char **argv);
void					b64_init(t_base64 *ctx
								, unsigned char *in
								, size_t in_len
								, int encoding);
void					b64_update(t_base64 *ctx
								, unsigned char *in
								, unsigned char *out
								, int encoding);
void					b64_final(t_base64 *ctx
								, unsigned char *in
								, unsigned char *out
								, int encoding);
unsigned char			*b64_full(unsigned char *in, size_t *len, int encoding);
void					b64_wrapper(void *input);
#endif
