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

# 既定ターゲット
all: $(NAME)

# リンク
$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) $(LDLIBS) -o $@

# 各 .c -> .o （依存関係ファイル生成）
%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -MMD -MP -c $< -o $@

# 依存関係読み込み
-include $(DEP)

clean:
	$(RM) $(OBJ) $(DEP)

fclean: clean
	$(RM) $(NAME)

re: fclean all

# オプション: デバッグ用
debug: CFLAGS += -g -O0
debug: re

.PHONY: all clean fclean re debug
