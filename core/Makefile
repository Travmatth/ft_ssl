# core Makefile

NAME = ft_ssl
CFLAGS = -Wall -Wextra -Werror -Wpedantic
CFLAGS += -I ../libftprintf/srcs/includes
LDFLAGS = -L ../libftprintf/ -lftprintf
LIBFT = ../libftprintf/libftprintf.a

FILES  := main

SRCS = $(wildcard objs/*.o)\
	$(addsuffix .c, $(FILES)) 

all: $(NAME)

$(NAME): 
	@$(CC) $(DEBUG) $(CFLAGS) $(LDFLAGS) $(LIBFT) -o $(NAME) $(SRCS)

clean:
	@echo -n 'Cleaning ft_ssl objs/ ... '
	@find . -name "*.o" -delete
	@echo "\033[32mdone\033[0m"

fclean: clean
	@echo -n 'Cleaning ft_ssl program... '
	@rm -rf $(NAME)
	@echo "\033[32mdone\033[0m"
	
re: fclean all

.PHONY: all clean fclean re