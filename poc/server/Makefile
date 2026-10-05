##
## EPITECH PROJECT, 2024
## RType
## File description:
## Makefile
##

SRC	=	src/Main.cpp \
		src/Server.cpp \
		src/UDPServer.cpp \
		src/ServArguments.cpp \
		src/System.cpp \
		src/GameRoom.cpp \
		src/Lobby.cpp \

OBJ	=	$(SRC:.cpp=.o)

NAME	=	r-type_server

RM	?=	rm -f

CXXFLAGS	=	-std=c++20 -Wall -Wextra -I./../../utils -g3

LDFLAGS	=	-lsfml-system

$(NAME):	$(OBJ)
	g++ -o $(NAME) $(OBJ) $(CXXFLAGS) $(LDFLAGS)

all:	$(NAME)
	mv $(NAME) ../../

clean:
	$(RM) $(OBJ)

fclean:	clean
	$(RM) ../../$(NAME)

re:	fclean all

.PHONY:	all clean fclean re