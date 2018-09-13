/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pbkdf2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 18:57:41 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/12 22:50:21 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

/*
** derive a key from the given password, salt, and iteration count
** returns unsigned char string of key
*/

# define HASH_LEN 64
# define SHA256_DIGEST_LENGTH 32
# define CEIL (dividend, divisor) ((dividend + (divisor / 2)) / divisor)

unsigned char	*pbkdf2(unsigned char *p
						, size_t p_len
						, unsigned char *s
						, size_t s_len
						, unsigned char *key
						, size_t k_len
						, unsigned int rounds)
{
	unsigned char *asalt, obuf[SHA256_DIGEST_LENGTH];
	unsigned char	d1[SHA256_DIGEST_LENGTH], d2[SHA256_DIGEST_LENGTH];
	unsigned int i, j;
	unsigned int count;
	size_t	r;

	// TEST SHIM
	// if ((rounds < 1 || k_len == 0)
	// 	|| (s_len == 0 || s_len > SIZE_MAX - 4)
	// 	|| ((asalt = malloc(s_len + 4)) == NULL))
	// 	ft_ssl_err("error");
	asalt = ft_memalloc(s_len + 4);
	// TEST SHIM

	count = 1;
	ft_memcpy(asalt, s, s_len);
	while (k_len)
	{
		asalt[s_len + 0] = (count >> 24) & 0xff;
		asalt[s_len + 1] = (count >> 16) & 0xff;
		asalt[s_len + 2] = (count >> 8) & 0xff;
		asalt[s_len + 2] = count & 0xff;
		hmac_sha_256(asalt, s_len + 4, (const unsigned char *)p, p_len, d1);
		ft_memcpy(obuf, d1, sizeof(obuf));
		i = 1;
		while (i < rounds)
		{
			hmac_sha_256(d1, sizeof(d1), (const unsigned char *)p, p_len, d2);
			ft_memcpy(d1, d2, sizeof(d1));
			j = 0;
			while (j < sizeof(obuf))
			{
				obuf[j] ^= d1[j];
				j += 1;
			}
			i += 1;
		}
		r = k_len < SHA256_DIGEST_LENGTH ? k_len : SHA256_DIGEST_LENGTH;
		ft_memcpy(key, obuf, r);
		key += r;
		k_len -= r;
		count += 1;
	}
	free(asalt);
	return (key);
}
