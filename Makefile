NAME = codexion

CC = cc

FLAGS = -Wall -Wextra -Werror -pthread -I${INC_DIR}

INC_DIR = ./includes/

INC = ${INC_DIR}codexion.h

SRCS = src/main.c \
	   src/utils.c \
	   src/parsing.c \
	   src/init.c \
	   src/threads.c \
	   src/monitor.c \
	   src/run.c \
	   src/cooldown.c \
	   src/scheduler.c \
	   src/heap.c

OBJS = ${SRCS:.c=.o}

all: ${NAME}

${NAME}: ${OBJS} ${INC}
	${CC} ${FLAGS} -o ${NAME} ${OBJS}

%.o: %.c ${INC}
		${CC} ${FLAGS} -o $@ -c $<

clean:
	rm -f ${OBJS}

fclean:
	rm -f ${NAME} ${OBJS}

re:
	fclean all

.PHONY: all clean fclean re