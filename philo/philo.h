#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>
# include <pthread.h>

# define MSG_NAME "phil"
# define ERRMSG_NUMARG "wrong number of arguments"
# define ERRMSG_INT "arguments must be positive integers"
# define ERRMSG_NUMPHIL "number of philosofers must put positive"
# define ERRMSG_TIMEDIE "time to die must put positive"
# define ERRMSG_TIMEEAT "time to eat must put positive"
# define ERRMSG_TIMESLE "time to sleep must put positive"
# define ERRMSG_MAXEAT "max number of eats must put positive or zero"
# define LOG_FORK "has taken a fork"
# define LOG_EAT "is eating"
# define LOG_SLEEP "is sleeping"
# define LOG_THINK "is thinking"
# define LOG_DIED "died"


# define DEBUG_MOD 1
# define NUM_EXTRA_MUTEX 2
# define NUM_EXTRA_THREADS 0

typedef struct s_args
{
	int	argc;
	int	num;
	int	die;
	int	eat;
	int	sleep;
	int	max_eat;
}	t_args;

typedef struct s_table
{
	t_args			*args;
	pthread_mutex_t	*m_print;
	pthread_mutex_t	*m_gener;
	pthread_mutex_t	*sticks;
	pthread_mutex_t	*m_phils;
	pthread_t		*philos;
	pthread_t		*waiter;
	struct s_sage	**guests;
	int				pasta_flag;
	int				num_eats;
}	t_table;

typedef struct s_sage
{
	t_table	*table;
	t_args	*args;
	int		pos;
	int		last_meal;
	int		num_eats;
	pthread_mutex_t	m_phil;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
}	t_sage;


long	ft_atoil(const char *str);
void	*ft_calloc(size_t count, size_t size);

int		ft_usleep(size_t milliseconds);
size_t	get_current_time(void);

#endif
