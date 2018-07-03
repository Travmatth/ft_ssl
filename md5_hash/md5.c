/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/30 20:13:01 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/03 15:17:42 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ssl.h"

t_buf	*md5(char *digest)
{
	t_buf	*b;
	size_t	len;

	len = LEN(digest, 0);
	if (!(b = ft_bufnew(digest, len, len)))
		ft_ssl_err("error");
	else
		ft_putendl("here");
	return (b);
}
