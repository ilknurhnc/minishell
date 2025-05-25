NAME = minishell

SRCS = 	minishell.c\
		utils.c\
		signals.c\
		quotes.c\
		init.c\
		dollar.c\
		control.c\
		bilmiyorum.c\
		env.c\
		setting.c\
		free.c\
		exec.c\
		redirect.c\
		
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