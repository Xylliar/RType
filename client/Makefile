##
## EPITECH PROJECT, 2024
## RType
## File description:
## Makefile
##

SRC	=	src/Main.cpp \
		src/Client.cpp \
		src/UDPClient.cpp \
		src/CliArguments.cpp \
		src/Menu.cpp \

OBJ	=	$(SRC:.cpp=.o)

NAME	=	r-type_client

RM	?=	rm -f

CXXFLAGS	=	-std=c++20 -Wall -Wextra -I./../utils -g3

LDFLAGS	=	-lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

$(NAME):	$(OBJ)
	g++ -o $(NAME) $(OBJ) $(CXXFLAGS) $(LDFLAGS)
	mv $(NAME) ../

all:	$(NAME)

clean:
	$(RM) $(OBJ)

fclean:	clean
	$(RM) ../$(NAME)

re:	fclean all
