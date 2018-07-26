DEBUG =
ifdef TEST
        DEBUG = -g -fsanitize=address -fsanitize=undefined
else
        DEBUG =
endif
ifdef LEAKS
        DEBUG = -g
endif

NAME = ft_ssl
LIBFT = libftprintf/libftprintf.a
CFLAGS += -Wall -Wextra -Werror -Wpedantic
LDFLAGS := -Llibftprintf -lftprintf -I./md5_hash -I./sha256_hash
FT_MD5 := md5 md5_hashing
FT_SHA256 := sha256 sha256_helpers
CORE := main hash_parsing hash_printing hash_padding
FILES := $(addprefix md5_hash/, $(FT_MD5)) \
	$(addprefix sha256_hash/, $(FT_SHA256)) \
	$(addprefix core/, $(CORE))
SRC := $(addsuffix .c, $(FILES))
OBJ := $(SRC:.c=.o)

.PHONY = all clean fclean re

all: $(SUBMODULES) $(NAME)

$(SUBMODULES):
	@git submodule init
	@git submodule update

$(LIBFT):
	@$(MAKE) -C libftprintf

$(OBJ): %.o: %.c
	@$(CC) -c $(DEBUG) -I. $(CFLAGS) $< -o $@

$(NAME): $(LIBFT) $(OBJ)
	@echo -n 'Compiling ft_ssl... '
	@$(CC) $(DEBUG) $(CFLAGS) $(LDFLAGS) $(OBJ) -o $@
	@echo "\033[32mdone\033[0m"

clean:
	@$(MAKE) clean -C libftprintf
	@echo -n 'Cleaning ft_ssl object files... '
	@rm -f $(OBJ) *.dSYM *.DS_Store
	@echo "\033[32mdone\033[0m"

fclean: clean
	@$(MAKE) fclean -C libftprintf
	@echo -n 'Cleaning ft_ssl executable... '
	@rm -f $(NAME)
	@echo "\033[32mdone\033[0m"

re: fclean all
