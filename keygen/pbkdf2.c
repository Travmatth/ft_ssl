/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pbkdf2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 18:57:41 by tmatthew          #+#    #+#             */
/*   Updated: 2018/08/16 19:19:50 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef struct DigestState
{
	uint64_t	len;
	union {
		uint32_t	state[16];
		uint64_t	bstate[8];
	};
	unsigned char	buf[256];
	int	blen;
	char	malloced;
	char	seeded;
}				DigestState;

void	pbkdf2_x(unsigned char *p
, uint32_t plen
, unsigned char *s
, uint32_t slen
, uint32_t rounds
, unsigned char *d
, uint32_t dlen
, DigestState* (*x)(unsigned char*, uint32_t, unsigned char*, uint32_t, unsigned char*, DigestState*)
, int xlen)
{
	unsigned char block[256], tmp[256];
	uint32_t		i;
	uint32_t		j;
	uint32_t		k;
	uint32_t		n;
	DigestState		*ds;

	i = 1;
	while (dlen)
	{
		tmp[3] = i;
		tmp[2] = i >> 8;
		tmp[1] = i >> 16;
		tmp[0] = i >> 24;
		ds = (*x)(s, slen, p, plen, nil, nil);
		(*x)(tmp, 4, p, plen, block, ds);
		ft_memmove(tmp, block, xlen);
		j = 1;
		while (j < rounds)
		{
			(*x)(tmp, xlen, p, plen, tmp, nil);
			k = 0;
			while (k < xlen)
			{
				block[k] ^= tmp[k];
				k += 1;
			}
			j += 1;
		}
		n = dlen > xlen ? xlen : dlen;
		ft_memmove(d, block, n); 
		i += 1;
		d += n;
		dlen -= n;
	}
}