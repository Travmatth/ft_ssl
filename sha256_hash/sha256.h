/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sha256.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 19:04:44 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/26 14:31:15 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHA256_H
# define SHA256_H

# define CH(x,y,z) (((x) & (y)) ^ (~(x) & (z)))
# define MAJ(x,y,z) (((x) & (y)) ^ ((x) & (z)) ^ ((y) & (z)))
# define EP0(x) (R_RIGHT(x,2) ^ R_RIGHT(x,13) ^ R_RIGHT(x,22))
# define EP1(x) (R_RIGHT(x,6) ^ R_RIGHT(x,11) ^ R_RIGHT(x,25))
# define SIG0(x) (R_RIGHT(x,7) ^ R_RIGHT(x,18) ^ ((x) >> 3))
# define SIG1(x) (R_RIGHT(x,17) ^ R_RIGHT(x,19) ^ ((x) >> 10))
# define W message_schedule

void			prepare_message_schedule(unsigned char *message
										, uint32_t *schedule);
unsigned char	*reverse_bytes(uint32_t *hash);
void			sha256_hash_init(uint32_t *hash_value);
void			append_len(unsigned char *out
							, size_t len
							, size_t orig_bit_len);
unsigned char	*sha256_preprocessing(char *pre_image, size_t *len);
#endif
