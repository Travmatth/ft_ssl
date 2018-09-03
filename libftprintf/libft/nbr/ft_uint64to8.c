/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_uint64to8.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/03 09:47:58 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/03 09:56:25 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
** The ft_uint64to8 function takes uint64_t and converts it to string of
** uint8_t characters
*/

uint8_t	*ft_uint64to8(uint8_t block, uint8_t *out)
{
	uint8_t		i;

	i = 0;
	while (i < 8)
	{
		out[i] = (uint8_t)(block >> (8 * (8 - (i + 1))));
		i += 1;
	}
	return (out);
}
