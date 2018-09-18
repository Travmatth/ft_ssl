/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   salsa.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 18:52:21 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/17 11:04:02 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SALSA_H
# define SALSA_H

# include "ft_ssl.h"

void	salsa_core(uint32_t in[16], uint32_t out[16], int rounds);
#endif
