/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/02 12:56:53 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/02 12:57:24 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

unsigned char		*prompt_for_password(void)
{
	char	*password;
	char	*verfication;

	password = getpass("enter des encryption password:");
	verfication = getpass("Verifying - enter des encryption password:");
	if (ft_strequ(password, verfication))
		return ((unsigned char*)password);
	else
		ft_ssl_err("error: passwords do not match");
}

unsigned char		*create_des_key(t_desctx *ctx)
{
	int			fd;
	char		*key;
	unsigned char		result[64];
	uint64_t	buf[2];

	if (!ctx->password)
		ctx->password = prompt_for_password();
	if (ctx->salt[0] == 0 && ctx->salt[1] == 0)
	{
		if (!(fd = open("/dev/random", O_RDONLY)))
			ft_ssl_err("error: cannot open /dev/random");
		if (!read(fd, buf, sizeof(uint64_t) * 2))
			ft_ssl_err("error occurred while reading /dev/random");
		ft_memcpy(ctx->salt, (void*)buf, sizeof(uint64_t));
	}
	scrypt(ctx->password, ctx->plen, ctx->salt, ctx->slen, 16, 1, 1, result, 64);
	ft_memcpy((void*)&key, (void*)result, sizeof(uint64_t));
	return ((unsigned char*)key);
}

int			ft_htouint64(uint8_t *hex, uint64_t *val)
{
	uint8_t		c;

	*val = 0;
	while (i < 16)
	{
		c = hex[i++];
		*val *= 16;
		if (c >= 'A' && c <= 'F')
			*val += c - 'A' + 10;
		else if (c >= 'a' && c <= 'f')
			*val += c - 'a' + 10;
		else if (c >= '0' && c <= '9')
			*val += c - '0';
		else
			return (0);
	}
	return (1);
}

uint64_t	ft_uint8to64(uint8_t *block)
{
	uint8_t		i;
	uint64_t	new;

	new = 0;
	i = 0;
	while (i < 8)
	{
		new |= ((uint64_t)block[i]) << (8 * (8 (i + 1)));
		i += 1;
	}
	return (new);
}

uint64_t	ft_uint64to8(uint8_t block, uint8_t *out)
{
	uint8_t		i;

	i = 0;
	while (i < 8)
	{
		out[i] = (uint8_t)(block >> (8 * (8 (i + 1))));
		i += 1;
	}
}
