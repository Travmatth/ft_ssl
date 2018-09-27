/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   printing.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/08 21:14:33 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/27 15:47:21 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

/*
** convert given hex values into into string
*/

char	*from_hex(char *output, unsigned char *hash_value, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		ft_snprintf(output + (i * 2), 3, "%02x\n", hash_value[i]);
		i += 1;
	}
	output[i * 2] = '\0';
	return (ft_strdup(output));
}

char	*from_hex_hash(char *output
							, unsigned char *hash_value
							, size_t len)
{
	size_t	i;

	i = 0;
	while (i < len)
	{
		ft_snprintf(output + (i * 2), 3, "%02x\n", hash_value[i]);
		i += 1;
	}
	output[i * 2] = '\0';
	free(hash_value);
	return (ft_strdup(output));
}

void	display_des_ciphers(void)
{
	ft_putendl("Cipher commands:");
	ft_putendl("des (defaults to des-ecb)");
	ft_putendl("des-ecb");
	ft_putendl("des-cbc");
	ft_putendl("des-pcbc");
	ft_putendl("des-cfb");
	ft_putendl("des-ofb");
	ft_putendl("des-ctr");
	ft_putendl("des3 (defaults to des3-cbc)");
	ft_putendl("des3-ecb");
	ft_putendl("des3-cbc");
	ft_putendl("des3-pcbc");
	ft_putendl("des3-cfb");
	ft_putendl("des3-ofb");
	ft_putendl("des3-ctr");
}
