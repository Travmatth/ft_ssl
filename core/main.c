/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/30 20:18:19 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/01 14:42:17 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ssl.h"

void	ft_ssl_usage(void)
{
	ft_putendl("message");
}

void	ft_ssl_err(const char *message)
{
	ft_putendl(message);
	exit(1);
}

int		main(void)
{
	ft_ssl_usage();
	md5("asdf");
	return (0);
}
