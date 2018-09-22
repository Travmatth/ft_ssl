/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_mode_parsing.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/21 20:11:11 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/21 20:58:33 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/ft_ssl.h"

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
	return (0);
}

int		parse_des_mode(t_desctx *ctx, char **argv, int *i)
{
	if (parse_des(ctx, argv, i))
		return (1);
	if (parse_des3(ctx, argv, i))
		return (1);
	return (0);
}
