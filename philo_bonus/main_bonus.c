#include "philo_bonus.h"
#include <semaphore.h>

// set_args.c
int	panic_args(char *msg, int return_value)
{
	printf("%s: %s\n", MSG_NAME, msg);
	return (return_value);
}

int	check_args(int argc, char **argv)
{
	int		i;
	char	*s;

	if (argc < 5 || 6 < argc)
		return (panic_args(ERRMSG_NUMARG, 1));
	i = 0;
	while (++i < argc)
	{
		s = argv[i];
		while (*s)
		{
			if (*s < '0' || '9' < *s)
				return (panic_args(ERRMSG_INT, 1));
			s++;
		}
	}
	return (0);
}

int	set_args(t_args *args, int argc, char **argv)
{
	if (check_args(argc, argv))
		return (1);
	args->argc = argc;
	args->table = NULL;
	args->num = ft_atoil(argv[1]);
	if (args->num < 1)
		return (panic_args(ERRMSG_NUMPHIL, 1));
	args->die = ft_atoil(argv[2]);
	if (args->die < 1)
		return (panic_args(ERRMSG_TIMEDIE, 1));
	args->eat = ft_atoil(argv[3]);
	if (args->eat < 1)
		return (panic_args(ERRMSG_TIMEEAT, 1));
	args->sleep = ft_atoil(argv[4]);
	if (args->sleep < 1)
		return (panic_args(ERRMSG_TIMESLE, 1));
	if (argc == 6)
		args->max_eat = ft_atoil(argv[5]);
	if (argc == 6 && args->max_eat <= 0)
		return (panic_args(ERRMSG_TIMEEAT, 1));
	if (argc == 5)
		args->max_eat = -1;
	return (0);
}

void print_debug_mode(int flag, t_args *args)
{
	if (flag)
	{
		printf("-----------   DEBUG MODE  -------- \n");
		printf("args are fine\n");
		printf("argc=%d\n", args->argc);
		printf("num=%d\n", args->num);
		printf("die=%d\n", args->die);
		printf("eat=%d\n", args->eat);
		printf("sleep=%d\n", args->sleep);
		printf("max_eat=%d\n", args->max_eat);
		printf("-----------   END OF DEBUG  -------- \n");
	}
}


/*
// routine_utils.c
void	reduce_num_eats(t_sage *sage)
{
	pthread_mutex_lock(sage->table->m_gener);
	if (sage->num_eats > 0)
		sage->num_eats--;
	pthread_mutex_unlock(sage->table->m_gener);
}

void	set_last_meal(t_sage *sage)
{
	pthread_mutex_lock(sage->table->m_gener);
	sage->last_meal = get_current_time();
	pthread_mutex_unlock(sage->table->m_gener);
}

int	continue_dinner(t_table *table)
{
	int	num_eats;
	int	pasta_flag;

	pthread_mutex_lock(table->m_gener);
	num_eats = table->num_eats;
	pasta_flag = table->pasta_flag;
	pthread_mutex_unlock(table->m_gener);
	if (pasta_flag == 0 || num_eats == 0)
		return (0);
	return (1);
}



// lockers.c
void	lock_right(t_sage *sage)
{
	pthread_mutex_lock(sage->right);
	sage->table->forks[sage->pos - 1] = 1;
}

void	unlock_right(t_sage *sage)
{
	sage->table->forks[sage->pos - 1] = 0;
	pthread_mutex_unlock(sage->right);
}

void	lock_left(t_sage *sage)
{
	int n;

	if (sage->pos == 1)
		n = sage->args->num - 1;
	else
		n = sage->pos - 2;
	pthread_mutex_lock(sage->left);
	sage->table->forks[n] = 1;
}

void	unlock_left(t_sage *sage)
{
	int n;

	if (sage->pos == 1)
		n = sage->args->num - 1;
	else
		n = sage->pos - 2;
	sage->table->forks[n] = 0;
	pthread_mutex_unlock(sage->left);
}



//routines.c
void	set_sticks(t_sage *sage)
{
	int	i;
	int	n;
	int left;

	i = sage->pos - 1;
	n = sage->args->num;
	left = i + (i == 0) * n - 1;
	sage->left = sage->table->sticks + left;
	sage->right = sage->table->sticks + i;
}

int	take_meal(t_sage *sage)
{
	//pthread_mutex_lock(sage->right);
	lock_right(sage);
	printf_mut(sage, LOG_FORK);
	if (sage->args->num >1)
	{
		lock_left(sage);
//		pthread_mutex_lock(sage->left);
		printf_mut(sage, LOG_FORK);
	}
	else 
		return (1);
	printf_mut(sage, LOG_EAT);
	set_last_meal(sage);
	ft_usleep(sage->args->eat);
	reduce_num_eats(sage);
	//pthread_mutex_unlock(sage->right);
	unlock_right(sage);
	if (sage->args->num > 1)
		unlock_left(sage);
//		pthread_mutex_unlock(sage->left);
	return (0);
}


*/

int print_return(char *msg, int ret)
{
	if (msg && *msg)
		printf("%s: %s\n", MSG_NAME, msg);
	return (ret);
}

int	printf_sem(t_sage *sage, char *str)
{
	if (sem_wait(sage->table->print) < 0)
		return (print_return("Error: sem_wait", EXIT_FAIL));
	if (1) //continue_dinner(sage->table))
		printf("%lu %d %s\n", get_current_time(), sage->pos, str);
	if (sem_post(sage->table->print) < 0)
		return (print_return("Error: sem_post", EXIT_FAIL));
	return (0);
}




int take_meal(t_sage *sage)
{
	ft_usleep(sage->pos);
//	printf("taking meal %d\n", sage->pos); //remove me
	if (sem_wait(sage->table->sticks) < 0)
		exit(print_return("Error: sem_wait", EXIT_FAIL));
	printf_sem(sage, LOG_FORK);
	if (sage->args->num > 1)
	{
		if (sem_wait(sage->table->sticks) < 0)
			exit(print_return("Error: sem_wait", EXIT_FAIL));
		printf_sem(sage, LOG_FORK);
	}
	else
		exit (1);
	printf_sem(sage, LOG_EAT);
	//set_last_meal(sage);
	//ft_usleep(sage->args->eat);
	if (sem_post(sage->table->sticks) < 0)// || sem_post(sage->table->sticks) < 0) 
	{
		printf("sem_post");
		exit(EXIT_FAIL);
	}
	printf("End of takeing meal %d\n", sage->pos);
	return (0);
}

int	routine(void *arg)
{
	t_sage	*sage;
	int	i;

	//printf("starting routine \n");
	sage = (t_sage *)arg;
	//ft_usleep(sage->pos);
//	if (DEBUG_MOD)
//		printf("Hi! I am %d\n", sage->pos);
	//set_sticks(sage);
//	if (sage->pos % 2)
//		ft_usleep(1);
//	exit(EXIT_SUCCESS);

	i = -1;
	while (++i < 5) //continue_dinner(sage->table))
	{
		take_meal(sage);
/*		if (take_meal(sage) || continue_dinner(sage->table) == 0)
			break ;
		printf_mut(sage, LOG_SLEEP);
		ft_usleep(sage->args->sleep);
		if (!continue_dinner(sage->table))
			break ;
		printf_mut(sage, LOG_THINK);
*/
	}
	exit (EXIT_SUCCESS);
}

/*
int	set_flags(t_table *tab)
{
	int		pasta_flag;
	int i;
	
	pasta_flag = 1;
	i = -1;
	tab->num_eats = 0;
	while (tab->guests[++i])
	{
		tab->num_eats += tab->guests[i]->num_eats;
		if (get_current_time() - \
			tab->guests[i]->last_meal >= tab->args->die)
			{
			tab->pasta_flag = 0;
			pasta_flag = 0;
			printf("%lu %d %s\n", get_current_time(),
				tab->guests[i]->pos, LOG_DIED);
			break ;
		}
	}
	return (pasta_flag);
}
*/

int	monitor(void *arg)
{
	//t_table	*tab;
	//int		pasta_flag;

	(void)arg;
	//tab = (t_table *)arg;
	//pasta_flag = 1;
	if (DEBUG_MOD)
		printf("Hi! I am waiter\n");
	/*
	while (tab->num_eats)
	{
		pthread_mutex_lock(tab->m_print);
		pthread_mutex_lock(tab->m_gener);
		pasta_flag = set_flags(tab);
		pthread_mutex_unlock(tab->m_print);
		pthread_mutex_unlock(tab->m_gener);
		if (pasta_flag == 0)
			break ;
		ft_usleep(1);
	}
	*/
	return (0);
}

// init_free.c
void	clean_guests(t_sage **guests)
{
	int	i;

	if (!guests)
		return ;
	i = -1;
	while (guests[++i])
	{
		free(guests[i]);
		guests[i] = NULL;
	}
	free(guests);
}

int clean_table_return(t_table *table, char *msg, int ret)
{
	if (!table)
		return (ret);
	if (table->philos)
		free(table->philos);
	table->philos = NULL;
	if (table->guests)
		clean_guests(table->guests);
	table->guests = NULL;
	if (msg && *msg)
		printf("%s: %s\n", MSG_NAME, msg);
	return (ret);
}

int	init_guests(t_sage **guests, t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->args->num)
	{
		guests[i] = (t_sage *)malloc(sizeof(t_sage));
		if (!guests[i])
		{
			while (i--)
				free(guests[i]);
			return (1);
		}
		guests[i]->pos = 0;
		guests[i]->num_eats = table->args->max_eat;
		guests[i]->table = table;
		guests[i]->args = table->args;
		guests[i]->last_meal = get_current_time();
		//printf("num i=%d is started at %lu\n", i, guests[i]->last_meal);
	}
	guests[i] = NULL;
	return (0);
}

void	set_table(t_table *table, t_args *args)
{
	table->args = args;
	table->pasta_flag = 1;
	table->num_eats = table->args->max_eat * table->args->num;
	table->sticks = NULL;
	table->print = NULL;
	table->gener = NULL;
	table->philos = NULL;
	table->guests = NULL;
}

int	init_table(t_table *table, t_args *args)
{
	set_table(table, args);
	table->philos = (pid_t *)malloc(sizeof(pid_t) * \
				(args->num + NUM_EXTRA_PROC));
	if (!table->philos)
		return (clean_table_return(table, "malloc err for table->philos", 1));
	table->waiter = table->philos + args->num;
	table->guests = (t_sage **)ft_calloc(args->num + 1, sizeof(t_sage *));
	if (!table->guests)
		return (1);
	if (init_guests(table->guests, table))
		return (clean_table_return(table, "malloc err in init_guests", 1));
	if (DEBUG_MOD)
		printf("table is initialized\n");
	return (0);
}


// main.c

int	make_processes(t_args *args, t_table *table)
{
	int	i;
	int status;

	i = -1;
	while (++i < args->num)
	{
		printf("i=%d\n", i);
		table->guests[i]->pos = i + 1;
		table->philos[i] = fork();
		if (table->philos[i] < 0)
			exit(clean_table_return(table, "fork error", 1));
		else if (i < args->num && table->philos[i] == 0)
			routine(table->guests[i]);
	}
	if (DEBUG_MOD)
		printf("Philosophers are now in the process\n");
	monitor(table);
	i = -1;
	while (++i < args->num)
		waitpid(table->philos[i], &status, 0);
	printf("main proccess is after waitpid\n");
	return ((status & 0xff00) >> 8);
}
/*{
	int	i;

	i = -1;
	while (++i < args->num)
	{
		table->guests[i]->pos = i + 1;
		if (pthread_create(table->philos + i, \
				NULL, &routine, table->guests[i]) != 0)
			return (clean_table_return(table, "error pthread_create", 1));
	}
	if (pthread_create(table->waiter, NULL, &monitor, table) != 0)
		return (clean_table_return(table, "error pthread_create", 1));
	i = -1;
	while (++i < args->num + NUM_EXTRA_THREADS)
	{
		if (pthread_join(table->philos[i], NULL) != 0)
			return (clean_table_return(table, "error pthread_join", 1));
	}
	return (0);
}
*/

int open_semaphores(t_table *table, t_args *args)
{
//	printf("args->num=%d\n", args->num); //remove me
	(void)args;
	table->sticks = sem_open(SEM_STICKS, O_CREAT, 0644, 10);// (unsigned int) args->num);
	if (table->sticks == SEM_FAILED)
	{
		printf("sem errror \n");//remove me
		return (clean_table_return(table, "error sem_open", 1));
	}
	//
	if (sem_trywait(table->sticks) == -1)
		printf("After open semaphore. It is closed\n");
	else
		printf("After open semaphore. It is open\n");
	//
	table->print = sem_open(SEM_PRINT, O_CREAT, 0644, 1);
	if (table->print == SEM_FAILED)
		return (clean_table_return(table, "error sem_open", 1));
	table->gener = sem_open(SEM_GENER, O_CREAT, 0644, 1);
	if (table->gener == SEM_FAILED)
		return (clean_table_return(table, "error sem_open", 1));
	if (DEBUG_MOD)
		printf("semaphores are open\n");
	return (0);
}

int close_semaphores(t_table *table)
{
	if ( \
		sem_close(table->sticks) < 0 || \
		sem_close(table->print) || \
		sem_close(table->gener))
		return (clean_table_return(table, "error sem_close", 1));
	if ( \
		sem_unlink(SEM_STICKS) < 0 || \
		sem_unlink(SEM_PRINT) || \
		sem_unlink(SEM_GENER))
		return (clean_table_return(table, "error sem_unlink", 1));
	if (DEBUG_MOD)
		printf("semaphores are closed and unlinked\n");
	return (0);
}

int	do_dinner(t_args *args)
{
	t_table	table;
	int		i;

	(void)i;
	if (init_table(&table, args))
		return (1);
	if (open_semaphores(&table, args))
		return (1);
	if (make_processes(args, &table))
		return (1);
	/*
	i = -1;
	while (++i < args->num + NUM_EXTRA_MUTEX)
	{
		if (pthread_mutex_destroy(table.sticks + i) == -1)
			return (clean_table_return(&table, "error pthread_mutex_destroy", 1));
	}
	*/
	if (close_semaphores(&table))
		return (1);
	clean_table_return(&table, NULL, 0);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_args	args;

	if (set_args(&args, argc, argv))
		return (1);
	if (argc == 6 && args.max_eat == 0)
		return (0);
	print_debug_mode(DEBUG_MOD, &args);
	if (do_dinner(&args))
		return (1);
	return (0);
}
