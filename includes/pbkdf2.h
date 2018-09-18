/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pbkdf2.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/16 19:29:01 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/17 11:03:53 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PBKDF2_H
# define PBKDF2_H

# include "ft_ssl.h"

typedef struct s_scrypt	t_scrypt;
void	pbkdf2(t_scrypt *opts, unsigned int rounds);
#endif
