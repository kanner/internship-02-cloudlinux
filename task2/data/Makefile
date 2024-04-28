NAME	= tester
SRC		= data-tester.cpp
OBJS	= ${SRC:.cpp=.o}
CXX		= clang++
CXXFLAGS	= -Wall -Wextra -Werror
RM		= rm -rf

all: ${NAME}

${NAME} : ${OBJS}
	${CXX} ${CXXFLAGS} ${OBJS} -o ${NAME}

%.o: %.cpp
	${CXX} ${CXXFLAGS} -c $< -o $@

clean:
	${RM} ${OBJS}
	${RM} ${NAME}

re: clean all

.PHONY: all clean re