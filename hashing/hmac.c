/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hmac.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 19:20:35 by tmatthew          #+#    #+#             */
/*   Updated: 2018/08/16 20:17:47 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

# define HMAC_BLOCK_SIZE 64

// called
// hmac_x(p, len, key, klen, digest, s, sha2_256, SHA2_256dlen);

unsigned char	*hmac_x(unsigned char *p
	, uint32_t len
	, unsigned char *key
	, uint32_t klen
	, unsigned char *digest
	, DigestState *s
	, DigestState*(*x)(unsigned char*, uint32_t, unsigned char*, DigestState*)
	, int xlen)
{
	int i;
	unsigned char innerdigest[256];
	unsigned char pad[HMAC_BLOCK_SIZE + 1];

	if (xlen > sizeof(innerdigest))
		return (NULL);
	if (klen > HMAC_BLOCK_SIZE){
		if (xlen > HMAC_BLOCK_SIZE)
			return (NULL);
		(*x)(key, klen, innerdigest, NULL);
		key = innerdigest;
		klen = xlen;
	}

	/* first time through */
	if(s == NULL || s->seeded == 0){
		memset(pad, 0x36, HMAC_BLOCK_SIZE);
		pad[HMAC_BLOCK_SIZE] = 0;
		for(i = 0; i < klen; i++)
			pad[i] ^= key[i];
		s = (*x)(pad, HMAC_BLOCK_SIZE, NULL, s);
		if(s == NULL)
			return NULL;
	}

	s = (*x)(p, len, NULL, s);
	if(digest == NULL)
		return s;

	/* last time through */
	memset(pad, 0x5c, HMAC_BLOCK_SIZE);
	pad[HMAC_BLOCK_SIZE] = 0;
	for(i = 0; i < klen; i++)
		pad[i] ^= key[i];
	(*x)(NULL, 0, innerdigest, s);
	s = (*x)(pad, HMAC_BLOCK_SIZE, NULL, NULL);
	(*x)(innerdigest, xlen, digest, s);
	return NULL;
}

// called
// ds = (*x)(s, slen, p, plen, nil, nil);
// (*x)(tmp, 4, p, plen, block, ds);

DigestState		*hmac_sha2_256(unsigned char *p
	, uint32_t len
	, unsigned char *key
	, uint32_t klen
	, unsigned char *digest
	, DigestState *s)
{
	return hmac_x(p, len, key, klen, digest, s, sha2_256, SHA2_256dlen);
}