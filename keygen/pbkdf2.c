/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pbkdf2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 18:57:41 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/15 11:10:44 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

/*
** derive a key from the given password, salt, and iteration count
** returns unsigned char string of key
*/

void	pbkdf2_rounds(t_hmac *hmac
, unsigned char *s
, size_t s_len
, unsigned char *key
, size_t *tmp
, unsigned int rounds)
{
	uint8_t count[4];
	uint8_t buf[SHA256_DIGEST_LEN];

	if (tmp[2] == tmp[0])
		tmp[5] = tmp[1];
	count[0] = (tmp[2] >> 24) & 0xFF;
	count[1] = (tmp[2] >> 16) & 0xFF;
	count[2] = (tmp[2] >> 8) & 0xFF;
	count[3] = tmp[2] & 0xFF;
	hmac_sha256_update(hmac, s, s_len);
	hmac_sha256_update(hmac, count, 4);
	hmac_sha256_final(hmac, buf);
	ft_memcpy(key, buf, tmp[5]);
	tmp[3] = 1;
	while (tmp[3]++ < rounds)
	{
		hmac_sha256_update(hmac, buf, SHA256_DIGEST_LEN);
		hmac_sha256_final(hmac, buf);
		tmp[4] = 0;
		while (tmp[4] < tmp[5])
		{
			key[tmp[4]] ^= buf[tmp[4]];
			tmp[4] += 1;
		}
	}
}

void	pbkdf2(uint8_t *p
						, size_t p_len
						, uint8_t *s
						, size_t s_len
						, uint8_t *key
						, size_t k_len
						, unsigned int rounds)
{
	size_t	tmp[6];
	t_hmac	hmac;

	tmp[5] = SHA256_DIGEST_LEN;
	tmp[0] = k_len / SHA256_DIGEST_LEN + ((k_len % SHA256_DIGEST_LEN) ? 1 : 0);
	tmp[1] = k_len - (tmp[0] - 1) * SHA256_DIGEST_LEN;
	tmp[2] = 1;
	hmac_sha256_init(&hmac, p, p_len);
	while (tmp[2] <= tmp[0])
	{
		pbkdf2_rounds(&hmac, s, s_len, key, tmp, rounds);
		key += tmp[5];
		tmp[2] += 1;
	}
}
