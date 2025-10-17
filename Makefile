NAME := philo

CC := cc

CFLAGS := -Wall -Wextra -Werror
CPPFLAGS := -I include
LDFLAGS :=
LDLIBS := -pthread

SRC := \
    src/main.c \
    src/init.c \
    src/simulation.c \
    src/utils.c \
    src/print.c \
    src/cleanup.c

OBJ := $(SRC:.c=.o)
DEP := $(OBJ:.o=.d)

all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) $(LDLIBS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MP -c $< -o $@

-include $(DEP)

clean:
	$(RM) $(OBJ) $(DEP)

fclean: clean
	$(RM) $(NAME)

re: fclean all

debug: CFLAGS += -g -O0
debug: re

.PHONY: all clean fclean re debug
