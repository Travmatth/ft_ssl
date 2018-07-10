/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/30 20:18:19 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/10 11:58:29 by tmatthew         ###   ########.fr       */
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

t_f		get_function(int *argc, char ***argv)
{
	size_t	i;

	i = 0;
	*argc -= 1;
	*argv = &argv[0][1];
	while (i < FT_SSL_FUNCS)
	{
		if (ft_strnequ(g_ptrs[i].cmd, argv[0][0], LEN(g_ptrs[i].cmd, 0)))
		{
			*argc -= 1;
			*argv = &argv[0][1];
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
	else if ((f = get_function(&argc, &argv)))
		f(parse_opts(argc, argv));
	return (0);
}
