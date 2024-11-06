#
# EPITECH PROJECT, 2024
# B-CNA-500-NAN-5-1-cryptography-toavina.andriamanampisoa
# File description:
# Makefile for the Gomoku project
#

#################--- SRC ---#################

BASE_SRC =


SRC	=	$(BASE_SRC)							\
		src/Main.cpp						\

TEST	=

#################--- OBJ ---#################

OBJ	=	$(SRC:.cpp=.o)

DEPS =	$(SRC:.cpp=.d)

TEST_OBJ	=	$(TEST:.cpp=.o)

#############################################

INCLUDE_PATH = -I ./inc -I ./ 

ERROR_FLAGS	=	-W -Wall -Wextra -Wshadow

CFLAGS = $(INCLUDE_PATH) $(ERROR_FLAGS) -g

NAME	=	pbrain-gomoku-ai

all:
			@echo -e "-----|\e[1;33mCompiling $(NAME)\e[0;m|-----"; \
			make start; \

start:	$(NAME)

$(NAME):	$(OBJ)
			g++ -o $(NAME) $(OBJ) $(CFLAGS) -lm -std=c++23
			@echo -e "-----|\e[1;34mdone\e[0;m|-----"

%.o: %.cpp
	@echo "Compilation of $<"
	@if g++ -c -o $@ $< $(CFLAGS) -std=c++23; then \
		echo -e "\033[0;34m[OK]\e[0;m | $@ <- $<"; \
	else \
		echo -e "\033[0;31m[FAILED]\e[0;m | $@ <- $<"; \
	fi

%.d: %.cpp
	@echo "Generating dependencies for $<"
	@$(CXX) $(CFLAGS) -MM -MT $(@:.d=.o) $< > $@


clean:
		@echo -e "-----|\e[1;33mClean $(NAME)\e[0;m|-----"
		rm -f $(OBJ)
		rm -f $(DEPS)
		rm -f ../$(NAME)
		rm -f $(NAME)
		@echo -e "-----|\e[1;34mdone\e[0;m|-----"

fclean:	clean
		rm -f ../$(NAME)

re:	fclean all

.PHONY: all clean fclean re tests_run
