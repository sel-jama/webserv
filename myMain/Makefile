SRCS = main.cpp server/Socket.cpp

OBJS = $(SRCS:.cpp=.o)

NAME = client

CC = c++

CPPFLAGS = -Wall -Wextra -Werror -std=c++98

all : ${NAME}

${NAME} : ${OBJS}
	${CC} ${CPPFLAGS} ${OBJS} -o ${NAME}

clean :
	rm -rf ${OBJS}

fclean : clean
	rm -rf ${NAME}

re : fclean all
