NAME = philo
CC = cc
CFLAGS = -Wall -Wextra -Werror
THREAD = -pthread

SRCS = 	srcs/main.c \
		srcs/init.c \
		srcs/utils.c \
		srcs/ft_atoi.c \
		srcs/routine.c \
		srcs/time.c \

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
		$(CC) $(CFLAGS) $(THREAD) -o $(NAME) $(OBJS)

clean:
		rm -f $(OBJS)

fclean: clean
		rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re