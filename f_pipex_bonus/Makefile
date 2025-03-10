NAME = ../pipex_bonus

CC = cc
CFLAGS = -Wall -Werror -Wextra

SRC = main.c					\
		path_utils.c			\
		cmd_execution.c			\
		cmd_parsing.c			\
		memory_management.c		\
		fd_management.c			\
		utils_and_errors.c		\
		here_doc.c				\
		get_next_line.c			\
		get_next_line_utils.c	\

HEADER = pipex_bonus.h	\
		get_next_line.h	\

OBJ = $(SRC:.c=.o)

all: $(NAME) 

$(NAME): $(OBJ) Makefile
	$(CC) $(OBJ) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re