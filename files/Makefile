# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: abinet <abinet@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/05/15 14:54:39 by acrespy           #+#    #+#              #
#    Updated: 2024/06/03 19:54:54 by abinet           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        := ircserv

SRCS		:= \
				srcs/channel/Channel.cpp \
				srcs/channel/Channel_utils.cpp \
				srcs/client/Client.cpp \
				srcs/client/Client_utils.cpp \
				srcs/commands/Server_botLog.cpp \
				srcs/commands/Server_invite.cpp \
				srcs/commands/Server_join.cpp \
				srcs/commands/Server_kick.cpp \
				srcs/commands/Server_mode.cpp \
				srcs/commands/Server_msg.cpp \
				srcs/commands/Server_nick.cpp \
				srcs/commands/Server_topic.cpp \
				srcs/commands/Server_user.cpp \
				srcs/server/Server.cpp \
				srcs/server/Server_close.cpp \
				srcs/server/Server_cmd.cpp \
				srcs/server/Server_run.cpp \
				srcs/server/Server_send.cpp \
				srcs/server/Server_setup.cpp \
				srcs/server/Server_signal.cpp \
				srcs/server/Server_utils.cpp \
				srcs/utils/ft_convert.cpp \
				srcs/utils/ft_print.cpp \
				srcs/utils/ft_signal.cpp \
				srcs/utils/ft_strlen.cpp \
				srcs/main.cpp

INCLUDES	:= \
			includes/channel/Channel.hpp \
			includes/client/Client.hpp \
			includes/headers/ft_irc_define.h \
			includes/headers/ft_irc_include.h \
			includes/headers/ft_irc_typedef.h \
			includes/server/Server.hpp \
			includes/utils/replies.h \
			includes/utils/utils.h \
			includes/ft_irc.h

OBJ_DIR     := .objs
OBJS 		:= ${SRCS:%.cpp=${OBJ_DIR}/%.o}

CC          := c++
CFLAGS      := -Wall -Wextra -Werror -g
CPPFLAGS	:= -std=c++98 -pedantic

RM          := rm -rf
MAKEFLAGS   += --no-print-directory
DIR_DUP     = mkdir -p ${@D}

all: ${NAME}

${NAME}: ${OBJS} ${INCLUDES}
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
.SILENT: ${NAME} ${SRCS} ${OBJS} ${INCLUDES} clean fclean re
