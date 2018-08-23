/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hmac.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 19:20:35 by tmatthew          #+#    #+#             */
/*   Updated: 2018/08/22 21:13:21 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

unsigned char		*hmac_sha_256(const unsigned char *text
								, size_t t_len
								, const unsigned char *key
								, size_t k_len
								, unsigned char digest[SHA256_DIGEST_LENGTH])
{
	unsigned char k_pad[SHA256_BLOCK_LENGTH];
	unsigned char tk[SHA256_DIGEST_LENGTH];
	unsigned char	*tmp[2];
	char	*out;
	int i;

	if (k_len > SHA256_BLOCK_LENGTH) {
		// SHA1Init(&ctx);
		// SHA1Update(&ctx, key, k_len);
		// SHA1Final(tk, &ctx);
		sha256_core((char*)tk, (char*)key);
		k_len = SHA256_DIGEST_LENGTH;
	}

	ft_memmove(k_pad, key, k_len);
	i = 0;
	while (i < SHA256_BLOCK_LENGTH)
		k_pad[i++] ^= 0x36;

	// SHA1Init(&ctx);
	// SHA1Update(&ctx, k_pad, SHA1_BLOCK_LENGTH);
	// SHA1Update(&ctx, text, t_len);
	// SHA1Final(digest, &ctx);
	if (!(tmp[0] = ft_memalloc(SHA256_BLOCK_LENGTH + t_len)))
		ft_ssl_err("error");
	ft_memcpy(tmp[0], k_pad, SHA256_BLOCK_LENGTH);
	ft_memcpy(tmp[0] + SHA256_BLOCK_LENGTH, text, t_len);
	sha256_core((char*)digest, (char*)tmp[0]);

	ft_memmove(k_pad, key, k_len);
	i = 0;
	while (i < SHA256_BLOCK_LENGTH)
		k_pad[i++] ^= 0x5c;

	// SHA1Init(&ctx);
	// SHA1Update(&ctx, k_pad, SHA1_BLOCK_LENGTH);
	// SHA1Update(&ctx, digest, SHA256_DIGEST_LENGTH);
	// SHA1Final(digest, &ctx);
	if (!(tmp[1] = ft_memalloc(SHA256_BLOCK_LENGTH + SHA256_DIGEST_LENGTH)))
		ft_ssl_err("error");
	ft_memcpy(tmp[1], k_pad, SHA256_BLOCK_LENGTH);
	ft_memcpy(tmp[1] + SHA256_BLOCK_LENGTH, digest, SHA256_DIGEST_LENGTH);
	if (!(out = ft_memalloc(SHA256_DIGEST_LENGTH)))
		ft_ssl_err("error");
	sha256_core((char*)out, (char*)digest);
	return ((unsigned char*)out);
}
