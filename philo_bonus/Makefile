NAME = philo_bonus

SRC =	main_bonus.c \
		atoil_bonus.c \
		ft_calloc_bonus.c \
		time_bonus.c

#FLAGS = -Wall -Wextra -Werror -pthread
FLAGS = -pthread #-fsanitize=thread

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
