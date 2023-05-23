#==============================================================================#
#                                   TARGETS                                    #
#==============================================================================#

NAME = webserv

#==============================================================================#
#                                   COMMANDS                                   #
#==============================================================================#

CXX = c++ -g3
CXXFLAGS = -Wall -Wextra -Werror -Wpedantic -pedantic-errors -MMD -MP #-fsanitize=address
MKDIR = mkdir -p
RM = rm -f
MAKE_SILENT = make --no-print-directory

#==============================================================================#
#                                    COLORS                                    #
#==============================================================================#

PURPLE = \033[35m
GREEN = \033[32m
YELLOW = \033[33m
END = \033[0m

#==============================================================================#
#                                    PATHS                                     #
#==============================================================================#

SRC_DIR = srcs/
HEADER_DIR = include/
OBJ_DIR = obj/

#==============================================================================#
#                                   SOURCES                                    #
#==============================================================================#

SRC = main.cpp \
		\
		cgi/CGI.cpp \
		\
		config/Config.cpp config/Location.cpp\
		\
		http/request/Request.cpp http/response/Response.cpp http/response/listing.cpp\
		http/response/upload.cpp\
		\
		server/Server.cpp server/Client.cpp \
		\
		utils/string.cpp \
		\

#==============================================================================#
#                                   HEADERS                                    #
#==============================================================================#

HEAD = $(SRC:.cpp=.hpp), include/webserv.hpp, srcs/utils, utils.hpp, Makefile

#==============================================================================#
#                                   OBJECTS                                    #
#==============================================================================#

OBJ = $(addprefix $(OBJ_DIR), $(SRC:.cpp=.o))
DEP = $(addprefix $(OBJ_DIR), $(SRC:.o=.d))

#==============================================================================#
#                                   MAKEFILE                                   #
#==============================================================================#

all : $(NAME)

$(OBJ_DIR)%.o : $(SRC_DIR)%.cpp
	mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(NAME) : $(OBJ) 
	echo "$(YELLOW)Making Webserv$(END)"
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(NAME)
	echo "$(GREEN)Done$(END)"

bonus : all

clean :
	echo "$(PURPLE)Cleaning Webserv's objects...$(END)"
	$(RM)r $(OBJ_DIR)
	echo "$(GREEN)Done$(END)"

fclean : clean
	echo "$(PURPLE)Cleaning Webserv...$(END)"
	$(RM) $(NAME)
	echo "$(GREEN)Done$(END)"

re : fclean
	$(MAKE_SILENT) all

exec:	all
		./${NAME}

#Dependencies list
-include ${DEP}

.PHONY : all clean fclean re exec
.SILENT :
