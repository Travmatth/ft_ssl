/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   usage.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/09/12 16:10:00 by tmatthew          #+#    #+#             */
/*   Updated: 2018/09/12 16:10:35 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_ssl.h"


/*
** You must include the following flags for DES:
** • -a, decode/encode the input/output in base64, depending on the encrypt mode
** • -d, decrypt mode
** • -e, encrypt mode (default)
** • -i, input file for message
** • -k, key in hex is the next arguement.
** (Behave like openssl des -K not openssl des -k)
** • -o, output file for message
** • -p, password in ascii is the next argument.
** (Behave like a modifiedd openssl des -pass not like openssl des -p or -P)
** • -s, the salt in hex is the next argument.
** (Behave like openssl des -S)
** • -v, initialization vector in hex is the next argument.
** (Behave like openssl des -iv not openssl des -v)
** If flags are not provided, be prepared to read/write from/to the console for the missing
** parameters similar to the behavior of OpenSSL.
**
** When the user does not have a cryptographically secure key, a new one must be created.
** This is why when a key is not provided, OpenSSL asks the user for a password. The key
** is generated using a Password-Based Key Derivation Function, or PBKDF.
**
** > openssl des-ecb
** enter des-ecb encryption password:
**
** To see the key that is generated from the password, you can use the
** -P flag.
** > openssl des-ecb -P
** enter des-ecb encryption password:
** Verifying - enter des-ecb encryption password:
** salt=EDCFEEFCA1850351
** key=914A103B0CE0A235
** >
** > openssl des-ecb -P
** enter des-ecb encryption password:
** Verifying - enter des-ecb encryption password:
** salt=3EB317A13C39A7D8
** key=DD334A3DE9C4C449
**
** To make your own keys from passwords, you will have to implement your own PBKDF.
** You must read data from STDIN (using your choice of the allowed functions above to
** prevent it showing up on the terminal) or read it as a flag. Your flag will of course be
** much cleaner than that of OpenSSL.
**
** > openssl des-ecb -pass "pass:MySuperSecurePassword"
** > ./ft_ssl des-ecb -p "MySuperSecurePassword"
**
** RFC 2898 sets PBKDF2 as the standard and RFC 8018 still endorses it.
** Another hint: re-creating OpenSSL’s PBKDF will not be very hard:
** it’s not really secure. Really.
**
** • A 64-bit long key. If a key is too short, pad it with zeros.
** Longer keys are truncated with the remainder discarded.
*/
