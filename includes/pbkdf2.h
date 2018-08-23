/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pbkdf2.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 19:29:01 by tmatthew          #+#    #+#             */
/*   Updated: 2018/08/22 22:10:59 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PBKDF2_H
# define PBKDF2_H

# include "ft_ssl.h"

unsigned char	*pbkdf2(unsigned char *p
						, size_t p_len
						, unsigned char *s
						, size_t s_len
						, unsigned char *key
						, size_t k_len
						, unsigned int rounds);
#endif
