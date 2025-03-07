NAME = ../pipex_bonus

CC = cc
CFLAGS = -Wall -Werror -Wextra

SRC = main.c					\
		path.c					\
		cmd_path.c				\
		free_functions.c		\
		executing.c				\
		parsing.c				\
		fd_managements.c			\
		utils.c					\
		here_doc.c				\
		get_next_line.c			\
		get_next_line_utils.c	\
		str_error.c				\

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