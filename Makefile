NAME = minishell

SRCS = 	minishell.c \
		utils.c \
		signals.c \
		init.c\
		parse/quotes.c \
		parse/dollar.c \
		parse/control.c \
		parse/seperate.c \
		parse/setting.c \
		parse/parse_utils.c \
		parse/argv_parser.c \
		builtin/builtin.c \
		builtin/env.c \
		builtin/export.c \
		builtin/unset.c \
		builtin/exit.c \
		builtin/cd.c \
		builtin/builtin_utils.c \
		execute/exec.c \
		execute/heredoc_utils.c\
		execute/redirect.c \
		execute/path.c \
		execute/redirect_control.c \
		execute/process_manage.c \
		free/free_1.c \
		free/free_2.c \
		
		
CC = cc
CFLAGS = -Wall -Wextra -Werror -g #-lhistory -lncurses #-fsanitize=address
OBJS = $(SRCS:.c=.o)
RM = rm -f

all: ${NAME}
${NAME}: ${OBJS}
	@${CC} ${CFLAGS} ${OBJS} -o ${NAME} -lreadline

leak:
	valgrind --leak-check=full --show-leak-kinds=all --suppressions=readline.supp ./minishell

clean: 
	@${RM} ${OBJS}

fclean: clean
	@${RM} ${NAME}

re: fclean all

.PHONY: all clean fclean re