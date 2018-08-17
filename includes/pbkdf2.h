/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pbkdf2.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 19:29:01 by tmatthew          #+#    #+#             */
/*   Updated: 2018/08/16 19:30:04 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef PBKDF2_H
# define PBKDF2_H

# include "ft_ssl.h"

void	pbkdf2_x(unsigned char *p
	, uint32_t plen
	, unsigned char *s
	, uint32_t slen
	, uint32_t rounds
	, unsigned char *d
	, uint32_t dlen
	, DigestState* (*x)(unsigned char*, uint32_t, unsigned char*, uint32_t, unsigned char*, DigestState*)
	, int xlen);
#endif
