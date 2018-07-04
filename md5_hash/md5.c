/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   md5.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/06/30 20:13:01 by tmatthew          #+#    #+#             */
/*   Updated: 2018/07/03 19:44:58 by tmatthew         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../ft_ssl.h"

/*
** we always read first from the stdin
** 	write to stdout w newline
** if only -p flag is given
** 	write message to stdout w newline
** 	write hash to stdout w newline
** if -q is given
** 	only echo the hash w newline, nothing else
** 		*if -p, still echo the given line
** if -r is given for file
** 	echo the hash
** 	followed by one space
** 	followed by the filename followed by newline
** if not an option, check if filename
** 	if file
** 		read file and write content hash to stdout w following syntax:
** 			`MD5 (<name>) = <hash>`
** 	else
** 		print: `ft_ssl: md5: <file> no such file or directory`
** if -s
** 	hash following string w syntax:
** 		`MD5 (<string>) = <hash>`
** order of evaluation:
** 	handle stdin
** 	file || string, whichever comes first
*/

void	md5(int argc, char **argv)
{
	t_digest	*input;

	input = parse_opts(argc, argv);
}
