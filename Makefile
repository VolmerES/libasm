NAME = libasm.a
SRC = ft_strlen.s
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
	gcc $(FLAGS) main.c $(NAME) -o test_program
	./test_program