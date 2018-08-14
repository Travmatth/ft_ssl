/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/07/21 19:04:44 by tmatthew          #+#    #+#             */
/*   Updated: 2018/08/13 19:18:33 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MD5_H
# define MD5_H

# include "ft_ssl.h"

uint32_t		f(uint32_t a, uint32_t b, uint32_t c);
uint32_t		g(uint32_t a, uint32_t b, uint32_t c);
uint32_t		h(uint32_t a, uint32_t b, uint32_t c);
uint32_t		i(uint32_t a, uint32_t b, uint32_t c);
#endif
