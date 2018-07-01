# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/17 18:33:21 by tmatthew          #+#    #+#              #
#    Updated: 2018/06/30 20:10:24 by tmatthew         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


DEBUG =
ifdef TEST
	DEBUG = -g -fsanitize=address -fsanitize=undefined
else
	DEBUG =
endif

FT_SSL = ft_ssl
LIBFT = libftprintf.a
TEST = test
CC = gcc

export CC
export DEBUG

.DEFAULT_GOAL := all
all: $(LIBFT) $(FT_PRINTF)

test: $(TEST)

.PHONY: $(LIBFT) $(MD5) $(FT_SSL) all

$(LIBFT):
	@$(MAKE) -C libftprintf 2>&1;

$(MD5):
	@$(MAKE) -C md5;

$(FT_SSL): $(LIBFT) $(MD5)
	@cp md5/*.o core/objs/
	@$(MAKE) -C core;

clean:
	@$(MAKE) clean -C libft;
	@$(MAKE) clean -C md5;
	@$(MAKE) clean -C core;

fclean: clean
	@rm -rf $(NAME) $(NAME).dSYM

re: fclean all
