/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 06:38:28 by apimikov          #+#    #+#             */
/*   Updated: 2024/04/19 12:23:32 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H
# include <stdio.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <semaphore.h>
# include <fcntl.h>
# include <signal.h>

# define MSG_NAME "phil"
# define ERRMSG_NUMARG "wrong number of arguments"
# define ERRMSG_INT "arguments must be positive integers and less then 8 digits"
# define ERRMSG_NUMPHIL "number of philosofers must put positive and less 201"
# define ERRMSG_TIMEDIE "time to die must put positive and less then hour"
# define ERRMSG_TIMEEAT "time to eat must put positive and less then hour"
# define ERRMSG_TIMESLE "time to sleep must put positive and less then hour"
# define ERRMSG_MAXEAT "max number of eats must put positive and less 60000"
# define LOG_FORK "has taken a fork"
# define LOG_EAT "is eating"
# define LOG_SLEEP "is sleeping"
# define LOG_THINK "is thinking"
# define LOG_DIED "died"

# define EXIT_FAIL 2

# define DEBUG_MOD 0
# define SEM_MOD 0
# define NUM_EXTRA_SEM 2

# define SEM_STICKS "/sticks"
# define SEM_PRINT  "/print"
# define SEM_GENER 	"/gener"
# define SEM_MEALS 	"/meals"

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
	sem_t			*print;
	sem_t			*gener;
	sem_t			*sticks;
	sem_t			*meals;
	int				*forks;
	pid_t			*philos;
	struct s_sage	**guests;
	int				num_eats;
	size_t			start_time;
}	t_table;

typedef struct s_sage
{
	t_table		*table;
	t_args		*args;
	int			pos;
	pthread_t	ghost;
	size_t		last_meal;
	int			num_eats;
	//sem_t	*left;
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
void	sem_wait_protected(sem_t *sem);
void	sem_post_protected(sem_t *sem);
int		printf_sem(t_sage *sage, char *str);
int		printf_time(t_sage *sage, char *str, size_t time);

//		routines.c
int		routine(void *arg);
void	*monitor(void *arg);

//		init_free.c
void	clean_guests(t_sage **guests);
int		clean_table_return(t_table *table, char *msg, int ret);
int		init_guests(t_sage **guests, t_table *table);
void	set_table(t_table *table, t_args *args);
int		init_table(t_table *table, t_args *args);

#endif
