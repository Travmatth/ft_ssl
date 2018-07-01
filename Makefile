# project makefile

DEBUG =
ifdef TEST
	DEBUG = -g -fsanitize=address -fsanitize=undefined
else
	DEBUG =
endif

FT_SSL = ft_ssl
LIBFT = libftprintf.a
CC = gcc

export CC
export DEBUG

.DEFAULT_GOAL := all
all: $(LIBFT) $(MD5) $(FT_SSL)

.PHONY: $(LIBFT) $(MD5) $(FT_SSL) all

$(LIBFT):
	@$(MAKE) -C libftprintf 2>&1;

$(MD5):
	@$(MAKE) -C md5;

$(FT_SSL): $(LIBFT) $(MD5)
	@$(MAKE) -C core;

clean:
	@$(MAKE) clean -C libftprintf;
	@$(MAKE) clean -C md5;
	@$(MAKE) clean -C core;

fclean: clean
	@rm -rf $(NAME) $(NAME).dSYM

re: fclean all
