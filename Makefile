NAME = pipex_bonus

CC = cc
CFLAGS = -Wall -Werror -Wextra

SRC = main.c			\
		path.c			\
		cmd_path.c		\
		free_functions.c	\
		executing.c		\
		parsing.c		\
		switch_fd.c		\
		utils.c			\

HEADER = pipex_bonus.h

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME) : $(OBJ) Makefile
	$(CC) $(OBJ) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CCF) -c $< -o $@

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re