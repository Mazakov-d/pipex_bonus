NAME = ../pipex

CC = cc
CFLAGS = -Wall -Werror -Wextra

SRCS = check_cmd.c		\
		executing.c		\
		get_path.c		\
		free_functions.c\
		str_error.c		\
		child_process.c \
		main.c			\

OBJS = $(SRCS:.c=.o)

HEADER = pipex.h

all: $(NAME) 

$(NAME): $(OBJS) Makefile
	$(CC) $(OBJS) -o $(NAME)

%.o: %.c $(HEADER)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re:	fclean all

.PHONY: all clean fclean re