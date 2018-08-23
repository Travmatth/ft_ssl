/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scrypt.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/15 19:21:44 by tmatthew          #+#    #+#             */
/*   Updated: 2018/08/22 22:11:31 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SCRYPT_H
# define SCRYPT_H

# include "ft_ssl.h"

void		scrypt(unsigned char *passphrase
	, size_t passphrase_len
	, unsigned char *salt
	, size_t salt_len
	, unsigned cost_param
	, unsigned block_size
	, unsigned parallelization_param
	, unsigned char *key
	, size_t key_len);
#endif
