# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ilandols <ilandols@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/14 14:14:06 by halvarez          #+#    #+#              #
#    Updated: 2023/03/31 18:43:56 by halvarez         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Default build version called with make rule = release
build	:= san

#Compilation of the mandatory part
NAME	= webserv

#Buildings objects
DIR		= ./srcs/
FILES	= main.cpp 
SRC		+= $(addprefix ${DIR}, ${FILES})

#Subdirectory example (c/p 3 lines)
SRV_DIR	= ${DIR}server/
SRV		= Server.cpp
SRC		+= $(addprefix ${SRV_DIR}, ${SRV})
##

OBJ		= ${SRC:.cpp=.o}
DEP		= ${OBJ:.o=.d}

CXX		= c++ -Wshadow -std=c++98

#Flag include directory (add -I[DIR])
CXX		+= -Isrcs/server 
RM		= rm -rf

#Conditionnal flags depending on building version
cxxflags.rls	:= -Wall -Wextra -Werror -Wpedantic -pedantic-errors -MMD -MP 
cxxflags.gdb	:= -g3 #-fstandalone-debug
cxxflags.san	:= -g3 -fsanitize=address #-fstandalone-debug 
CXXFLAGS		= ${cxxflags.rls} ${cxxflags.${build}}
export			CXXFLAGS

%.o : %.cpp
		${CXX} ${CXXFLAGS} -c $< -o $@

#Mandatory rules
all:    ${NAME}

${NAME}:${OBJ}
		${CXX} ${CXXFLAGS} ${OBJ} -o ${NAME}

#Execution rule
run:	all
		./${NAME}

#Cleaning rules
clean:
		${RM} ${OBJ} ${DEP}

fclean: clean
		${RM} ${NAME}

re:     fclean
		${MAKE} all

#Dependencies list
-include ${DEP}

.PHONY: all clean fclean re bonus run
