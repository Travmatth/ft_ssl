/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 19:04:44 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/14 19:22:03 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHA256_H
# define SHA256_H

#define SHA256_BLOCK_LEN  64ul
#define SHA256_DIGEST_LEN 32ul
#define SHA256_DIGEST_INT 8

# define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
# define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
# define EP0(x) (R_RIGHT(x,2) ^ R_RIGHT(x,13) ^ R_RIGHT(x,22))
# define EP1(x) (R_RIGHT(x,6) ^ R_RIGHT(x,11) ^ R_RIGHT(x,25))
# define SIG0(x) (R_RIGHT(x,7) ^ R_RIGHT(x,18) ^ ((x) >> 3))
# define SIG1(x) (R_RIGHT(x,17) ^ R_RIGHT(x,19) ^ ((x) >> 10))
# define W message_schedule

typedef struct	s_sha256
{
	size_t		len;
	uint32_t	state[8];
	uint8_t		buf[64];
}				t_sha256;

void			sha256_pad_plaintext(t_sha256 *ctx);
void			sha256_rounds(uint32_t hash_value[8], unsigned char *message);
void			sha256_init(t_sha256 *ctx);
void			sha256_update(t_sha256 *ctx, uint8_t *message, size_t len);
void			sha256_final(t_sha256 *ctx, uint8_t *digest);
char			*sha256_full(uint8_t *in, size_t len);
void			sha256_ssl_wrapper(void *input);
#endif
