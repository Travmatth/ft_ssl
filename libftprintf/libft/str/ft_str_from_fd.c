/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_count_by_delim.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 16:15:17 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/21 18:35:19 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

/*
** reads a given fd and returns a string with its contents
*/

char		*ft_str_from_fd(int fd)
{
	size_t	len;
	size_t	bytes;
	char	*tmp;
	char	*string;
	char	buf[BUFF_SIZE];

	bytes = 0;
	string = NULL;
	while ((bytes = read(fd, buf, BUFF_SIZE)) != 0)
	{
		if (string)
		{
			len = LEN(string, 0);
			if (!(tmp = ft_strnew(len + bytes)))
				return (NULL);
			ft_memcpy(tmp, string, len);
			ft_memcpy(tmp + len, buf, bytes);
			free(string);
			string = tmp;
		}
		else
			string = ft_strndup(buf, bytes);
	}
	return (string);
}
