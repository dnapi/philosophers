NAME = philo

SRC =	main.c \
		atoil.c \
		ft_calloc.c \
		time.c \
		set_args.c \
		routines_utils.c \
		lockers.c \
		routines.c \
		init_free.c

FLAGS = -Wall -Wextra -Werror -pthread
#FLAGS = -pthread -fsanitize=thread

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
