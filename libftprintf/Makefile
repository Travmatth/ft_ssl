# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: tmatthew <tmatthew@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2018/05/17 18:33:21 by tmatthew          #+#    #+#              #
#    Updated: 2018/06/30 16:37:22 by tmatthew         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


DEBUG =
ifdef TEST
	DEBUG = -g -fsanitize=address -fsanitize=undefined
else
	DEBUG =
endif

LIBFT = libft
FT_PRINTF = srcs
TEST = test
CC = gcc

export CC
export DEBUG

.DEFAULT_GOAL := all
all: $(LIBFT) $(FT_PRINTF)

test: $(TEST)

.PHONY: $(LIBFT) $(FT_PRINTF) $(TEST) all

$(LIBFT):
	@$(MAKE) -C libft 2>&1;

$(FT_PRINTF):
	@$(MAKE) -C srcs;

$(TEST): $(LIBFT) $(FT_PRINTF)
	@$(MAKE) -C test;

test: $(TEST)

clean:
	@$(MAKE) clean -C libft;
	@$(MAKE) clean -C srcs;
	@$(MAKE) clean -C test;

fclean:
	@$(MAKE) fclean -C libft
	@$(MAKE) fclean -C srcs;
	@$(MAKE) fclean -C test;
	@rm -rf libftprintf.a

re: fclean all
