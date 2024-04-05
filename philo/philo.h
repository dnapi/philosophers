#ifndef PHILO_H
# include <stdio.h>

# define MSG_NAME "phil"
# define ERRMSG_NUMARG "wrong number of arguments"
# define ERRMSG_INT "arguments must be integers"
# define ERRMSG_NUMPHIL "number of philosofers must put positive"
# define ERRMSG_TIMEDIE "time to die must put positive"
# define ERRMSG_TIMEEAT "time to eat must put positive"
# define ERRMSG_TIMESLE "time to sleep must put positive"

# define DEBUG_MOD 1

typedef struct s_philo
{
    int num;
    int die;
    int eat;
    int sleep;
    int max_eat;
}   t_philo;

long    ft_atoil(const char *str);

#endif
