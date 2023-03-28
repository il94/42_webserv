# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: halvarez <halvarez@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/02/14 14:14:06 by halvarez          #+#    #+#              #
#    Updated: 2023/03/28 14:45:50 by halvarez         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

#Default build version called with make rule = release
build	:= san

#Compilation of the mandatory part
NAME	= webser

#Buildings objects
DIR		=
FILES	= main.cpp 
SRC		+= $(addprefix ${DIR}, ${FILES})

#Subdirectory example (c/p 3 lines)
CLS_DIR	= ./cls/
CLS		= cls.cpp cls2/cls2.cpp
SRC		+= $(addprefix ${CLS_DIR}, ${CLS})
##

OBJ		= ${SRC:.cpp=.o}
DEP		= ${OBJ:.o=.d}

CXX		= c++ -Wshadow -std=c++98

#Flag include directory (add -I[DIR])
CXX		+= -Icls -Icls/cls2
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
