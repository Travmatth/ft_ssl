/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/13 11:01:21 by tmatthew          #+#    #+#             */
/*   Updated: 2018/08/22 22:01:11 by tmatthew         ###   ########.fr       */
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

unsigned char		*create_des_key(t_des_state *state)
{
	int			fd;
	char		*key;
	unsigned char		result[64];
	uint64_t	buf[2];

	if (!state->password)
		state->password = prompt_for_password();
	if (state->salt[0] == 0 && state->salt[1] == 0)
	{
		if (!(fd = open("/dev/random", O_RDONLY)))
			ft_ssl_err("error: cannot open /dev/random");
		if (!read(fd, buf, sizeof(uint64_t) * 2))
			ft_ssl_err("error occurred while reading /dev/random");
		ft_memcpy(state->salt, (void*)buf, sizeof(uint64_t));
	}
	scrypt(state->password
		, LEN((char *)state->password, 0)
		, state->salt
		, LEN((char *)state->salt, 0)
		, 16
		, 1
		, 1
		, result
		, 64);
	ft_memcpy((void*)&key, (void*)result, sizeof(uint64_t));
	return ((unsigned char*)key);
}

void		des(void *input)
{
	t_des_state	*state;

	state = (t_des_state*)input;
	if (!state->key)
		state->key = create_des_key(state);
}
