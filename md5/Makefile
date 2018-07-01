# md5 Makefile

MD5  := md5
CFLAGS = -c -Wall -Wextra -Werror -Wpedantic
SRCS = md5.c
HEADER = ../ft_ssl.h

all: $(MD5)

$(MD5):
	@$(CC) $(HEADER) $(CFLAGS) $(SRCS)

clean:
	@echo -n 'Cleaning md5 functions... '
	@find . -name "*.o" -delete
	@echo "\033[32mdone\033[0m"

fclean: clean
	
re: fclean all

.PHONY: all clean fclean re