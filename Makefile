NAME := philo

CC := cc
CFLAGS := -Wall -Wextra -Werror -I include

SRC := \
    src/main.c \
    src/init.c \
    src/init_philo.c \
    src/parse.c \
    src/simulation.c \
    src/routine.c \
    src/monitor.c \
    src/utils.c \
    src/print.c \
    src/cleanup.c

OBJ := $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -pthread -o $@

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re