/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_mode_parsing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 20:11:11 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/24 10:35:13 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

/*
** Parsing functionality needed to set des struct member permute hooks
** to the functions necessary to support various block cipher chaining modes
*/

t_desopt	g_desopts[] =
{
	{"des", des_null_permute_hook, des_null_permute_hook, 0},
	{"des-ecb", des_null_permute_hook, des_null_permute_hook, 0},
	{"des-cbc", des_cbc_pre_permute_hook, des_cbc_post_permute_hook, 1},
	{"des-pcbc", des_pcbc_pre_permute_hook, des_pcbc_post_permute_hook, 1},
	{"des-cfb", des_cfb_pre_permute_hook, des_cfb_post_permute_hook, 1},
	{"des-ofb", des_ofb_pre_permute_hook, des_ofb_post_permute_hook, 1},
	{"des-ctr", des_ctr_pre_permute_hook, des_ctr_post_permute_hook, 1},
	{"des3-cfb", des_cfb_pre_permute_hook, des_cfb_post_permute_hook, 1},
	{"des3-ofb", des_ofb_pre_permute_hook, des_ofb_post_permute_hook, 1},
	{"des3-ctr", des_ctr_pre_permute_hook, des_ctr_post_permute_hook, 1},
	{"des3", des_null_permute_hook, des_null_permute_hook, 0},
	{"des3-ecb", des_null_permute_hook, des_null_permute_hook, 0},
	{"des3-cbc", des_cbc_pre_permute_hook, des_cbc_post_permute_hook, 1},
	{"des3-pcbc", des_pcbc_pre_permute_hook, des_pcbc_post_permute_hook, 1}
};

int		parse_des_mode(t_desctx *ctx, char **argv, int *i)
{
	size_t	n;

	n = 0;
	while (n < 13)
	{
		if (ft_strequ(g_desopts[n].name, argv[*i]))
		{
			ctx->pre_permute_chaining = g_desopts[n].pre_permute_chaining;
			ctx->post_permute_chaining = g_desopts[n].post_permute_chaining;
			if (g_desopts[n].needs_v)
				SET_NEED_V(ctx->flags);
			return (1);
		}
		n += 1;
	}
	return (0);
}
