NAME = philo

SRC =	main.c\
			atoil.c

FLAGS = -Wall -Wextra -Werror

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ) 
	cc $(FLAGS) $(OBJ) -o $(NAME) 

%.o: %.c
	cc $(FLAGS) -c $^ -o $@
	
clean:
	rm -f ${OBJ}

fclean: clean
	rm -f ${NAME}

re: fclean all

.PHONY: all clean fclean re
