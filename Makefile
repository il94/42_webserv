# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ilandols <ilandols@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/14 14:14:06 by halvarez          #+#    #+#              #
#    Updated: 2023/04/07 16:03:32 by ilandols         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Default build version called with make rule = release
build	:= #an

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

CF_DIR	= ${DIR}config_file/
CF		= configFile.cpp Config.cpp
SRC		+= $(addprefix ${CF_DIR}, ${CF})

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
		${CXX} -c $< -o $@

#Mandatory rules
all:    ${NAME}

${NAME}:${OBJ}
		${CXX} ${OBJ} -o ${NAME}

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
