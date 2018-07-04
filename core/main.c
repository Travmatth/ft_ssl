/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/30 20:18:19 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/03 19:45:13 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ssl.h"

t_ptrs g_ptrs[] =
{
	{"md5", &md5},
};

void	ft_ssl_usage(void)
{
	ft_putendl("usage: ft_ssl command [command opts] [command args]");
	exit(1);
}

void	ft_ssl_err(const char *message)
{
	ft_putendl(message);
	exit(1);
}

t_f		get_function(int *argc, char **argv)
{
	size_t	i;

	i = 0;
	while (i < FT_SSL_FUNCS)
	{
		if (ft_strequ(g_ptrs[i].cmd, argv[1]))
		{
			*argc -= 2;
			*argv = argv[2];
			return (g_ptrs[i].f);
		}
		i += 1;
	}
	return (NULL);
}

int		main(int argc, char **argv)
{
	t_f	f;

	if (argc == 1)
		ft_ssl_usage();
	else if ((f = get_function(&argc, argv)))
		f(argc, argv);
	return (0);
}
