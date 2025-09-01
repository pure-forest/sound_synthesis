NAME=minisynth
CC=c++
FLAGS=-g
AUDIO=-lpulse-simple -lpulse
INC=$(addprefix inc/, \
		parsing.hpp \
		SoundMaking.hpp)

SRC= $(addprefix src/, \
		main.cpp \
		parsing.cpp \
		SoundMaking.cpp	\
		math.cpp )

OBJ=$(SRC:src/%.cpp=obj/%.o)

all: bin/$(NAME)

obj/%.o: src/%.cpp $(INC)
	@mkdir -p $(dir $@)
	$(CC) $(FLAGS) -o  $@ -c $<

bin/$(NAME):$(OBJ)
	@mkdir -p $(dir $@)
	$(CC) $(OBJ) -o bin/$(NAME) $(AUDIO)

clean:
	rm -rf obj/

fclean:clean
	rm -rf $(NAME) bin/

re:fclean all

.PHONY: all clean fclean  re
