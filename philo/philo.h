/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 06:38:28 by apimikov          #+#    #+#             */
/*   Updated: 2024/04/14 06:43:28 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
# define ERRMSG_MAXEAT "max number of eats must put positive"
# define LOG_FORK "has taken a fork"
# define LOG_EAT "is eating"
# define LOG_SLEEP "is sleeping"
# define LOG_THINK "is thinking"
# define LOG_DIED "died"

# define DEBUG_MOD 0
# define NUM_EXTRA_MUTEX 2
# define NUM_EXTRA_THREADS 1

typedef struct s_args
{
	int				argc;
	int				num;
	int				die;
	int				eat;
	int				sleep;
	int				max_eat;
	struct s_table	*table;
}	t_args;

typedef struct s_table
{
	t_args			*args;
	pthread_mutex_t	*m_print;
	pthread_mutex_t	*m_gener;
	pthread_mutex_t	*sticks;
	int				*forks;
	pthread_mutex_t	*m_phils;
	pthread_t		*philos;
	pthread_t		*waiter;
	struct s_sage	**guests;
	int				pasta_flag;
	int				num_eats;
}	t_table;

typedef struct s_sage
{
	t_table			*table;
	t_args			*args;
	int				pos;
	size_t			last_meal;
	int				num_eats;
	pthread_mutex_t	m_phil;
	pthread_mutex_t	*left;
	pthread_mutex_t	*right;
}	t_sage;

//atoil.c
long	ft_atoil(const char *str);
//ft_calloc.c
void	*ft_calloc(size_t count, size_t size);

//time.c
int		ft_usleep(size_t milliseconds);
size_t	get_current_time(void);

//		set_args.c
int		set_args(t_args *args, int argc, char **argv);
void	print_debug_mode(int flag, t_args *args);

//		routine_utils.c
void	reduce_num_eats(t_sage *sage);
void	set_last_meal(t_sage *sage);
int		continue_dinner(t_table *table);
void	printf_mut(t_sage *sage, char *str);

//		lockers.c
void	lock_right(t_sage *sage);
void	unlock_right(t_sage *sage);
void	lock_left(t_sage *sage);
void	unlock_left(t_sage *sage);

//		routines.c
void	*routine(void *arg);
void	*monitor(void *arg);

//		init_free.c
void	clean_guests(t_sage **guests);
int		clean_table_return(t_table *table, char *msg, int ret);
int		init_guests(t_sage **guests, t_table *table);
void	set_table(t_table *table, t_args *args);
int		init_table(t_table *table, t_args *args);

#endif
