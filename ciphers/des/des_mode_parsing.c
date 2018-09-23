/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_mode_parsing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 20:11:11 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/22 19:38:24 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

/*
** Parsing functionality needed to set des struct member permute hooks
** to the functions necessary to support various block cipher chaining modes
*/

int		parse_des_extended(t_desctx *ctx, char **argv, int *i)
{
	if (ft_strequ("des-cfb", argv[*i]))
	{
		ctx->pre_permute_chaining = des_cfb_pre_permute_hook;
		ctx->post_permute_chaining = des_cfb_post_permute_hook;
		SET_NEED_V(ctx->flags);
		return (1);
	}
	else if (ft_strequ("des-ofb", argv[*i]))
	{
		ctx->pre_permute_chaining = des_ofb_pre_permute_hook;
		ctx->post_permute_chaining = des_ofb_post_permute_hook;
		SET_NEED_V(ctx->flags);
		return (1);
	}
	else if (ft_strequ("des-ctr", argv[*i]))
	{
		ctx->pre_permute_chaining = des_ctr_pre_permute_hook;
		ctx->post_permute_chaining = des_ctr_post_permute_hook;
		SET_NEED_V(ctx->flags);
		return (1);
	}
	return (0);
}

int		parse_des3_extended(t_desctx *ctx, char **argv, int *i)
{
	if (ft_strequ("des3-cfb", argv[*i]))
	{
		ctx->pre_permute_chaining = des_cfb_pre_permute_hook;
		ctx->post_permute_chaining = des_cfb_post_permute_hook;
		SET_NEED_V(ctx->flags);
		return (1);
	}
	else if (ft_strequ("des3-ofb", argv[*i]))
	{
		ctx->pre_permute_chaining = des_ofb_pre_permute_hook;
		ctx->post_permute_chaining = des_ofb_post_permute_hook;
		SET_NEED_V(ctx->flags);
		return (1);
	}
	else if (ft_strequ("des3-ctr", argv[*i]))
	{
		ctx->pre_permute_chaining = des_ctr_pre_permute_hook;
		ctx->post_permute_chaining = des_ctr_post_permute_hook;
		SET_NEED_V(ctx->flags);
		return (1);
	}
	return (0);
}

int		parse_des(t_desctx *ctx, char **argv, int *i)
{
	if (ft_strequ("des-ecb", argv[*i]) || ft_strequ("des", argv[*i]))
	{
		ctx->pre_permute_chaining = des_null_permute_hook;
		ctx->post_permute_chaining = des_null_permute_hook;
		return (1);
	}
	else if (ft_strequ("des-cbc", argv[*i]))
	{
		ctx->pre_permute_chaining = des_cbc_pre_permute_hook;
		ctx->post_permute_chaining = des_cbc_post_permute_hook;
		SET_NEED_V(ctx->flags);
		return (1);
	}
	else if (ft_strequ("des-pcbc", argv[*i]))
	{
		ctx->pre_permute_chaining = des_pcbc_pre_permute_hook;
		ctx->post_permute_chaining = des_pcbc_post_permute_hook;
		SET_NEED_V(ctx->flags);
		return (1);
	}
	return (0);
}

int		parse_des3(t_desctx *ctx, char **argv, int *i)
{
	if (ft_strequ("des3-ecb", argv[*i]) || ft_strequ("des3", argv[*i]))
	{
		ctx->pre_permute_chaining = des_null_permute_hook;
		ctx->post_permute_chaining = des_null_permute_hook;
		return (1);
	}
	else if (ft_strequ("des3-cbc", argv[*i]))
	{
		ctx->pre_permute_chaining = des_cbc_pre_permute_hook;
		ctx->post_permute_chaining = des_cbc_post_permute_hook;
		SET_NEED_V(ctx->flags);
		return (1);
	}
	else if (ft_strequ("des3-pcbc", argv[*i]))
	{
		ctx->pre_permute_chaining = des_pcbc_pre_permute_hook;
		ctx->post_permute_chaining = des_pcbc_post_permute_hook;
		SET_NEED_V(ctx->flags);
		return (1);
	}
	return (0);
}

int		parse_des_mode(t_desctx *ctx, char **argv, int *i)
{
	if (parse_des(ctx, argv, i))
		return (1);
	else if (parse_des_extended(ctx, argv, i))
		return (1);
	else if (parse_des3(ctx, argv, i))
		return (1);
	else if (parse_des3_extended(ctx, argv, i))
		return (1);
	return (0);
}
