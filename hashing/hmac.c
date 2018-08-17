/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hmac.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 19:20:35 by tmatthew          #+#    #+#             */
/*   Updated: 2018/08/16 19:43:52 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

DigestState*	hmac_x(unsigned char *p
	, uint32_t len
	, unsigned char *key
	, uint32_t klen
	, unsigned char *digest
	, DigestState *s
	, DigestState*(*x)(unsigned char*, uint32_t, unsigned char*, DigestState*)
	, int xlen)
{
	int i;
	unsigned char pad[Hmacblksz+1], innerdigest[256];

	if(xlen > sizeof(innerdigest))
		return NULL;
	if(klen > Hmacblksz){
		if(xlen > Hmacblksz)
			return NULL;
		(*x)(key, klen, innerdigest, NULL);
		key = innerdigest;
		klen = xlen;
	}

	/* first time through */
	if(s == NULL || s->seeded == 0){
		memset(pad, 0x36, Hmacblksz);
		pad[Hmacblksz] = 0;
		for(i = 0; i < klen; i++)
			pad[i] ^= key[i];
		s = (*x)(pad, Hmacblksz, NULL, s);
		if(s == NULL)
			return NULL;
	}

	s = (*x)(p, len, NULL, s);
	if(digest == NULL)
		return s;

	/* last time through */
	memset(pad, 0x5c, Hmacblksz);
	pad[Hmacblksz] = 0;
	for(i = 0; i < klen; i++)
		pad[i] ^= key[i];
	(*x)(NULL, 0, innerdigest, s);
	s = (*x)(pad, Hmacblksz, NULL, NULL);
	(*x)(innerdigest, xlen, digest, s);
	return NULL;
}

DigestState		*hmac_sha2_256(unsigned char *p
	, uint32_t len
	, unsigned char *key
	, uint32_t klen
	, unsigned char *digest
	, DigestState *s)
{
	return hmac_x(p, len, key, klen, digest, s, sha2_256, SHA2_256dlen);
}