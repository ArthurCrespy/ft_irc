# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: acrespy <acrespy@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/15 14:54:39 by acrespy           #+#    #+#              #
#    Updated: 2023/05/16 15:01:53 by acrespy          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        := ircserv

SRCS		:= \
				srcs/client/Client.cpp \
				srcs/server/Server.cpp \
				srcs/server/Server_setup.cpp \
				srcs/server/Server_start.cpp \
				srcs/server/Server_utils.cpp \
				srcs/utils/ft_convert.cpp \
				srcs/utils/ft_print.cpp \
				srcs/main.cpp \

OBJ_DIR     := .objs
OBJS 		:= ${SRCS:%.cpp=${OBJ_DIR}/%.o}

CC          := c++
CFLAGS      := -Wall -Wextra -Werror -g
CPPFLAGS	:= -std=c++98 -pedantic

RM          := rm -rf
MAKEFLAGS   += --no-print-directory
DIR_DUP     = mkdir -p ${@D}

all: ${NAME}

${NAME}: ${OBJS}
	${CC} ${OBJS} -o ${NAME}
	${info EXECUTABLE CREATED: ${NAME}}

${OBJ_DIR}/%.o : %.cpp
	${DIR_DUP}
	${CC} ${CFLAGS} ${CPPFLAGS} -c $< -o $@
	${info OBJECT CREATED: $@}

clean:
	${RM} ${OBJS} ${OBJ_DIR}
	${info REMOVED OBJECTS: ${OBJS} ${OBJ_DIR}}

fclean: clean
	${RM} ${NAME}
	${info REMOVED EXECUTABLES: ${NAME}}

re:
	${MAKE} fclean
	${MAKE} all

.PHONY: all clean fclean re
.SILENT: ${NAME} ${OBJS} ${SRCS} ${IRSSI} clean fclean re