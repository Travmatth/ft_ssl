/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/30 20:18:19 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/25 12:35:59 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"

t_ptrs g_ptrs[] =
{
	{"md5", &md5, &parse_hash_opts},
	{"sha256", &sha256_ssl_wrapper, &parse_hash_opts},
	{"base64", &b64_wrapper, &parse_b64_opts},
	{"des", &des_wrapper, &parse_des_opts},
	{"des-ecb", &des_wrapper, &parse_des_opts},
	{"des-cbc", &des_wrapper, &parse_des_opts},
	{"des-pcbc", &des_wrapper, &parse_des_opts},
	{"des-cfb", &des_wrapper, &parse_des_opts},
	{"des-ofb", &des_wrapper, &parse_des_opts},
	{"des-ctr", &des_wrapper, &parse_des_opts},
	{"des3", &des3_wrapper, &parse_des_opts},
	{"des3-ecb", &des3_wrapper, &parse_des_opts},
	{"des3-cbc", &des3_wrapper, &parse_des_opts},
	{"des3-pcbc", &des_wrapper, &parse_des_opts},
	{"des3-cfb", &des_wrapper, &parse_des_opts},
	{"des3-ofb", &des_wrapper, &parse_des_opts},
	{"des3-ctr", &des_wrapper, &parse_des_opts}
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

void	ft_ssl_cmd_err(char *command)
{
	char	*err;

	err = "Error: '%s' is an invalid command\n";
	ft_putendl((err = ft_swap(err, "%s", command)));
	free(err);
	ft_putendl("Standard commands:");
	ft_putendl("");
	ft_putendl("Encoding commands:");
	ft_putendl("base64");
	ft_putendl("");
	ft_putendl("Message Digest commands:");
	ft_putendl("md5");
	ft_putendl("sha256");
	ft_putendl("");
	display_des_ciphers();
	exit(1);
}

int		get_function(int *argc, char ***argv, t_f *f, t_p *p)
{
	size_t	i;

	i = 0;
	*argc -= 1;
	*argv = &argv[0][1];
	while (i < FT_SSL_FUNCS)
	{
		if (ft_strnequ(g_ptrs[i].cmd, argv[0][0], LEN(g_ptrs[i].cmd, 0)))
		{
			if (!ft_strnequ(argv[0][0], "des", 3))
			{
				*argc -= 1;
				*argv = &argv[0][1];
			}
			*f = g_ptrs[i].f;
			*p = g_ptrs[i].p;
			return (1);
		}
		i += 1;
	}
	ft_ssl_cmd_err((char*)argv[0][0]);
	return (0);
}

int		main(int argc, char **argv)
{
	t_f		f;
	t_p		p;

	if (argc == 1)
		ft_ssl_usage();
	else if (get_function(&argc, &argv, &f, &p))
		f(p(argc, argv));
	return (0);
}
