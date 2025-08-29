NAME=minisynth
CC=c++
FLAGS=-Wall -Wextra -Werror -g
INC=$(addprefix inc/, \
		parsing.hpp)

SRC= $(addprefix src/, \
		main.cpp \
		parsing.cpp	)

OBJ=$(SRC:src/%.cpp=obj/%.o)

all: bin/$(NAME)

obj/%.o: src/%.cpp $(INC)
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) -o $@ -c $<

bin/$(NAME):$(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(OBJ) -o bin/$(NAME)

clean:
	rm -rf obj/

fclean:clean
	rm -rf $(NAME) bin/

re:fclean all

.PHONY: all clean fclean  re
