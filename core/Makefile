# core Makefile

# NAME = ft_ssl
# CFLAGS = -Wall -Wextra -Werror -Wpedantic
# CFLAGS += -I ../libftprintf/srcs/includes
# LDFLAGS = -L ../libftprintf/ -lftprintf
# LIBFT = ../libftprintf/libftprintf.a

# FILES  := main

# SRCS = $(addsuffix .c, $(FILES)) 

# all: $(NAME)

# $(NAME): 
# 	$(CC) $(DEBUG) $(CFLAGS) $(LDFLAGS) $(LIBFT) -o $(NAME) ../md5/md5.o $(SRCS)

# clean:
# 	@echo -n 'Cleaning ft_ssl objs/ ... '
# 	@find . -name "*.o" -delete
# 	@echo "\033[32mdone\033[0m"

# fclean: clean
# 	@echo -n 'Cleaning ft_ssl program... '
# 	@rm -rf $(NAME)
# 	@echo "\033[32mdone\033[0m"
	
# re: fclean all

# .PHONY: all clean fclean re
#######
NAME = ft_ssl
CFLAGS = -Wall -Wextra -Werror -Wpedantic
LIBFT = -L ../libftprintf/srcs/includes ../libftprintf/libftprintf.a

SRC = main.c

all: $(SRC)
	@echo -n 'Compiling ft_ssl ... '
	@$(CC) $(DEBUG) $(CFLAGS) $(LIBFT) -o $(NAME) ../md5_hash/md5.o $<
	@echo "\033[32mdone\033[0m"

clean:
	@echo -n 'Cleaning test object files... '
	@find . -name "*.o" -delete
	@echo "\033[32mdone\033[0m"

fclean: clean
	@echo -n 'Cleaning test main... '
	@rm -rf $(NAME) $(NAME).dSYM
	@echo "\033[32mdone\033[0m"

re: fclean all%