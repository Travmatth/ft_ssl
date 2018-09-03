/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/02 12:56:53 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/03 14:28:15 by tmatthew         ###   ########.fr       */
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

uint8_t		*create_des_key(t_desctx *ctx)
{
	int			fd;
	uint8_t		*key;
	uint8_t		result[64];
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
	if (!(key = ft_memalloc(ctx->klen)))
		ft_ssl_err("error");
	ft_memcpy((void*)&key, (void*)result, sizeof(uint64_t));
	return (key);
}
