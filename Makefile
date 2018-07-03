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
MD5 = md5_hash/md5.o

export CC
export DEBUG

.DEFAULT_GOAL := all
all: $(FT_SSL)

.PHONY: $(LIBFT) $(MD5) $(FT_SSL) all

$(LIBFT):
	@echo "LIBFT here"
	@$(MAKE) -C libftprintf 2>&1;

$(MD5):
	@echo "MD5 here"
	@$(MAKE) -C md5_hash;
	@cp md5_hash/*.o core/objs

$(FT_SSL): $(LIBFT) $(MD5)
	@echo "FT_SSL here"
	@$(MAKE) -C core;

clean:
	@$(MAKE) clean -C libftprintf;
	@$(MAKE) clean -C md5_hash;
	@$(MAKE) clean -C core;

fclean:
	@$(MAKE) fclean -C libftprintf;
	@$(MAKE) fclean -C md5_hash;
	@$(MAKE) fclean -C core;
	@rm -rf $(NAME) $(NAME).dSYM

re: fclean all