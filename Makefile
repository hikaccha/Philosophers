NAME := philo

CC := cc
CFLAGS := -Wall -Wextra -Werror
THREADFLAGS := -pthread

INCLUDES := -I include

SRC := \
  src/main.c \
  src/init.c \
  src/simulation.c \
  src/utils.c \
  src/print.c \
  src/cleanup.c

OBJ := $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(SRC) include/philo.h
	$(CC) $(CFLAGS) $(THREADFLAGS) $(SRC) $(INCLUDES) -o $(NAME)

clean:
	@true

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re


