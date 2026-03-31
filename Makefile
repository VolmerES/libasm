NAME = libasm.a
SRC = ft_strlen.s ft_strcpy.s ft_strcmp.s
OBJ = $(SRC:.s=.o)

# -f elf64 para Linux, -f macho64 si estás en un Mac de 42
NASM = nasm -f macho64
FLAGS = -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.s
	$(NASM) $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME) test_program

re: fclean all

test: all
	bash ./run_tests.sh
	@$(MAKE) fclean >/dev/null

.PHONY: all clean fclean re test
