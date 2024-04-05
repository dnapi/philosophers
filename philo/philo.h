#ifndef PHILO_H
# include <stdio.h>

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
