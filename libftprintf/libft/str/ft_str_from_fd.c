/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_str_from_fd.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/04/23 16:15:17 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/17 18:57:02 by tmatthew         ###   ########.fr       */
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
			ft_memcpy((char*)ft_memcpy(tmp, string, len) + len, buf, bytes);
			free(string);
			string = tmp;
		}
		else
			string = ft_strndup(buf, bytes);
		if (bytes < BUFF_SIZE)
			break ;
	}
	return (string);
}
