/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lsttail.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/05/11 12:18:44 by tmatthew          #+#    #+#             */
/*   Updated: 2018/05/19 18:29:04 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft.h"

t_list	*ft_lsttail(t_list **begin_list)
{
	t_list	*tmp;

	if (!(begin_list && *begin_list))
		return (NULL);
	if (!(*begin_list)->next)
	{
		tmp = *begin_list;
		*begin_list = NULL;
	}
	while ((*begin_list)->next && (*begin_list)->next->next)
		(*begin_list) = (*begin_list)->next;
	(*begin_list) = (*begin_list)->next;
	tmp = (*begin_list)->next;
	(*begin_list)->next = NULL;
	return (tmp);
}
