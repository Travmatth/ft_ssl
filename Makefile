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
LDFLAGS := -Llibftprintf -lftprintf -I./includes
MD5 := md5 md5_hashing
DES := des_algo des_helpers des_modes des des_parsing
SALSA := salsa
SHA256 := sha256 sha256_algo
KEYGEN := pbkdf2 scrypt
BASE64 := b64_parsing b64 b64_algo
CIPHERS := $(addprefix des/, $(DES)) \
	$(addprefix salsa/, $(SALSA))
HASHING := hmac hash_padding hash_parsing hash_printing \
	$(addprefix md5_hash/, $(MD5)) \
	$(addprefix sha256_hash/, $(SHA256))
CORE := main parsing printing
FILES := $(addprefix core/, $(CORE)) \
	$(addprefix hashing/, $(HASHING)) \
	$(addprefix b64/, $(BASE64)) \
	$(addprefix ciphers/, $(CIPHERS)) \
	$(addprefix keygen/, $(KEYGEN))
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
