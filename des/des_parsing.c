/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   des_parsing.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/08/13 11:01:23 by tmatthew          #+#    #+#             */
/*   Updated: 2018/08/13 20:20:09 by tmatthew         ###   ########.fr       */
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
** (A verbose explanation is given in the next section)
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
** The DES algorithm works by splitting the plaintext into blocks of 64 bits (8 chars) in
** length, and performing a set of operations on those blocks with a 64-bit key. The process
** it performs on each block is the same, and by itself this is simply called ECB mode.
** The requirements of ECB mode are:
** • A 64-bit long key. If a key is too short, pad it with zeros.
** For example, hex key FF12CD becomes FF12CD0000000000.
** Hex key FF1 becomes FF10000000000000.
** Longer keys are truncated with the remainder discarded.
** • A 64-bit long block. If a block is too short, pad it with the size difference byte
** padding scheme, the same as OpenSSL.
** To avoid all that password nonsense, you can input a key directly
** with the -k flag.
** > ./ft_ssl des-ecb -k 01020304FEFDFCFB
** You may use the -nopad flag with OpenSSL while testing to make sure
** your algorithm is correct before checking the padding.
** > echo "foo bar" | openssl des-ecb -K 6162636461626364 -a -nopad
** YZF3QKaabXU=
** Initialization Vector is not used for ECB mode, but can still be
** taken and not used.
** 11
** ft_ssl [base64] [des] The real fun: symmetric encoding and encryption
** V.0.5 Correcting Broken Ciphers
** The previous exercise had your DES algorithm operate in ECB mode, meaning each encrypted
** block was concatenated to the end of the block before it. For this next part, you
** must implement BC mode, or Cipher Block Chaining. Rather than simply concatenating
** the next block, each block is also XOR’d with the block before it.
** This is where the -v flag comes in for the Initialization Vector.
** > echo "one deep secret" | ./ft_ssl des-cbc -a -k 6162636461626364 -v 0011223344556677
** zqYWONX68rWNxl7msIdGC67Uh2HfVEBo
** >
** > echo "zqYWONX68rWNxl7msIdGC67Uh2HfVEBo" | openssl des-cbc -d -a -K 6162636461626364 -iv
** 0011223344556677
** one deep secret
** Your DES-CBC must operate in the real CBC mode, where the IV is
** modified after each block.
** You must be able to encrypt and decrypt all modes with excecutables
** made by other students and the OpenSSL executable.
*/

/*
** parses next option and modifies state with flag or string to be hashed
*/

typedef struct	s_des_state
{
	uint64_t	key;
	uint64_t	salt;
	uint64_t	initialization_vector;
	int			out_file;
	char		*message;
	char		*password;
	char		flags[BITNSLOTS(4)];
}				t_des_state;

# define SET_DECRYPT(v) (BITSET(v, 0))
# define SET_ENCRYPT(v) (BITSET(v, 1))
# define SET_INPUT(v) (BITSET(v, 2))
# define SET_A(v) (BITSET(v, 3))

# define GET_DECRYPT(v) (BITTEST(v, 0))
# define GET_ENCRYPT(v) (BITTEST(v, 1))
# define GET_INPUT(v) (BITTEST(v, 2))
# define GET_A(v) (BITTEST(v, 3))

static void	parse_base64_opts_handler(t_des_state *state
	, char **argv
	, int *i)
{
	int		fd;

	if (ft_strequ("-a", argv[*i]))
		SET_A(state->flags);
	else if (ft_strequ("-d", argv[*i]))
		SET_DECRYPT(state->flags);
	else if (ft_strequ("-e", argv[*i]))
		SET_ENCRYPT(state->flags);
	else if (ft_strequ("-i", argv[*i]))
	{
		if (!argv[*i + 1])
			ft_ssl_err("error");
		if (!(fd = open(argv[*i + 1], O_RDONLY)))
			ft_ssl_err("error: cannot find file");
		state->message = ft_str_from_fd(fd);
		*i += 1;
		SET_INPUT(state->flags);
	}
	else if (ft_strequ("-k", argv[*i]))
	{
		if (!argv[*i + 1])
			ft_ssl_err("error");
		state->key = ft_htoi(argv[*i + 1]);
		*i += 1;
	}
	else if (ft_strequ("-o", argv[*i]))
	{
		if (!argv[*i + 1])
			ft_ssl_err("error");
		if (!(state->out_file = open(argv[*i + 1], O_WRONLY)))
			ft_ssl_err("error");
		*i += 1;
	}
	else if (ft_strequ("-p", argv[*i]))
	{
		if (!argv[*i + 1])
			ft_ssl_err("error");
		state->password = ft_strdup(argv[*i + 1]);
		*i += 1;
	}
	else if (ft_strequ("-s", argv[*i]))
	{
		if (!argv[*i + 1])
			ft_ssl_err("error");
		state->salt = ft_htoi(argv[*i + 1]);
		*i += 1;
	}
	else if (ft_strequ("-s", argv[*i]))
	{
		if (!argv[*i + 1])
			ft_ssl_err("error");
		state->initialization_vector = ft_htoi(argv[*i + 1]);
		*i += 1;
	}
}

/*
** accepts the argc & argv arguments, parses and returns
** a struct holding string to be hashed
*/

void		*parse_des_opts(int argc, char **argv)
{
	int			i;
	t_des_state	state;

	i = -1;
	ft_bzero(&state, sizeof(t_des_state));
	while (++i < argc)
		parse_base64_opts_handler(&state, argv, &i);
	if (!GET_INPUT(state.flags))
		state.message = ft_str_from_fd(STDIN);
	return (ft_memcpy(ft_memalloc(sizeof(t_des_state))
		, &state
		, sizeof(t_base64)));
}
