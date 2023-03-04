# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: lrosa-do <lrosa-do@student.42lisboa>       +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/01/20 18:22:02 by lrosa-do          #+#    #+#              #
#    Updated: 2023/03/03 17:23:21 by lrosa-do         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME =		main

CXX =		g++
CPPFLAGS =	-g -Wall   -Wshadow
#CPPFLAGS +=  -fsanitize=address 
#CPPFLAGS += -o3 -s

CPPLDFLAGS =	   -lSDL2  -lGL -lm 

OBJS_DIR	=	obj
SRCS_DIR	=	src


SRCS		=	main.cpp core.cpp render.cpp utils.cpp math.cpp glad.cpp stb_image.cpp
SRC_PATH 	=  src/
OBJS		=	$(addprefix ${OBJS_DIR}/, ${SRCS:.cpp=.o})


RM			=	rm -rf
MKDIR		=	mkdir -p

all:		$(NAME)

$(NAME):	$(OBJS)
			@$(CXX)      $(OBJS) -o $(NAME) $(CPPLDFLAGS)
			@echo "Linked executable \033[38;2;39;154;186m$(NAME)\033[0m"
			@./$(NAME) 
			rm -f obj/main.o

${OBJS_DIR}/%.o: 	${SRCS_DIR}/%.cpp	
			@${MKDIR} $(@D)
			$(CXX) -I./include  $(CPPFLAGS)  -c $< -o $@ 
			@echo "\e[91m" " Compile " "\e[0m ($<)" "\e[91m" "  object" "\e[0m ($@)"

clean:
			@$(RM) $(OBJS) $(OBJS_DIR)
			@echo "\e[91m" " Clean  obs" "\e[0m"

fclean:		clean
			@$(RM) $(NAME)
			@echo "\e[91m" " Delete executable" "\e[0m"

re:			fclean all

.PHONY:		fclean, all, re