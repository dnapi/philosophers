#include "philo_bonus.h"

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

void	print_debug_mode(int flag, t_args *args)
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


// routine_utils.c
int	print_return(char *msg, int ret)
{
	if (msg && *msg)
		printf("%s: %s\n", MSG_NAME, msg);
	return (ret);
}

void	sem_wait_protected(sem_t *sem)
{
	if (sem_wait(sem) < 0)
		exit(print_return("Error: sem_wait", EXIT_FAIL));
}

void	sem_post_protected(sem_t *sem)
{
	if (sem_post(sem) < 0)
		exit(print_return("Error: sem_post", EXIT_FAIL));
}

int	printf_sem(t_sage *sage, char *str)
{
	sem_wait_protected(sage->table->print);
	printf("%lu %d %s\n", get_current_time(), sage->pos, str);
	sem_post_protected(sage->table->print);
	return (0);
}

int	printf_time(t_sage *sage, char *str, size_t time)
{
	sem_wait_protected(sage->table->print);
	printf("%lu %d %s\n", time, sage->pos, str);
	sem_post_protected(sage->table->print);
	return (0);
}


//routine.c
void	set_print_meal(t_sage *sage)
{
	sem_wait_protected(sage->table->gener);
	sage->last_meal = get_current_time();
	printf_time(sage, LOG_EAT, sage->last_meal);
	sem_post_protected(sage->table->gener);
}

void	*monitor(void *arg)
{
	t_sage	*sage;
	int		i;

	sage = (t_sage *)arg;
	if (DEBUG_MOD)
		printf("Hi! I am ghost of %d\n", sage->pos);
	while (1)
	{
		sem_wait_protected(sage->table->gener);
		i = get_current_time() - sage->last_meal >= (size_t)sage->args->die;
		if (i)
		{
			sem_wait_protected(sage->table->print);
			printf("%lu %d %s\n", get_current_time(), sage->pos, LOG_DIED);
			i = -1;
			while (++i < sage->args->num * sage->args->max_eat)
				sem_post_protected(sage->table->meals);
			if (sage->args->max_eat == -1)
				sem_post_protected(sage->table->meals);
			break ;
		}
		sem_post_protected(sage->table->gener);
		ft_usleep(10);
	}
	return (arg);
}

int	take_meal(t_sage *sage)
{
	sem_wait_protected(sage->table->sticks);
	printf_sem(sage, LOG_FORK);
	if (sage->args->num > 1)
	{
		sem_wait_protected(sage->table->sticks);
		printf_sem(sage, LOG_FORK);
	}
	else
	{
		ft_usleep(sage->args->die * 2);
		return (0);
	}
	set_print_meal(sage);
	ft_usleep(sage->args->eat);
	sem_post_protected(sage->table->sticks);
	sem_post_protected(sage->table->sticks);
	if (sage->num_eats < sage->args->max_eat)
	{
		sage->num_eats++;
		sem_post_protected(sage->table->meals);
	}
	return (0);
}

int	routine(void *arg)
{
	t_sage	*sage;

	sage = (t_sage *)arg;
	if (pthread_create(&(sage->ghost), NULL, &monitor, sage) != 0)
		exit (clean_table_return(sage->table, "error pthread_create", \
			EXIT_FAIL));
	if (sage->args->num > 1 && sage->pos % 2)
	{
		printf_sem(sage, LOG_THINK);
		ft_usleep(5);
	}
	if (sage->args->num > 1 && \
		sage->pos == sage->args->num && \
		sage->args->num % 2)
		ft_usleep(5);
	while (1)
	{
		take_meal(sage);
		printf_sem(sage, LOG_SLEEP);
		ft_usleep(sage->args->sleep);
		printf_sem(sage, LOG_THINK);
	}
	if (pthread_join(sage->ghost, NULL) != 0)
		exit (clean_table_return(sage->table, "err pthread_join", EXIT_FAIL));
	exit (EXIT_SUCCESS);
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

int	clean_table_return(t_table *table, char *msg, int ret)
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
		guests[i]->num_eats = 0;
		guests[i]->table = table;
		guests[i]->args = table->args;
		guests[i]->last_meal = get_current_time();
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
	table->philos = (pid_t *)malloc(sizeof(pid_t) * args->num);
	if (!table->philos)
		return (clean_table_return(table, "malloc err for table->philos", 1));
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
	int	status;

	i = -1;
	while (++i < args->num)
	{
		table->guests[i]->pos = i + 1;
		table->philos[i] = fork();
		if (table->philos[i] < 0)
			exit(clean_table_return(table, "fork error", 1));
		else if (table->philos[i] == 0)
			routine(table->guests[i]);
	}
	i = -1;
	while (++i < args->num * args->max_eat)
		sem_wait_protected(table->meals);
	if (args->max_eat == -1)
		sem_wait_protected(table->meals);
	i = -1;
	while (++i < args->num)
		kill(table->philos[i], SIGTERM);
	i = -1;
	while (++i < args->num)
		waitpid(table->philos[i], &status, 0);
	return ((status & 0xff00) >> 8);
}

int	open_semaphores(t_table *table, t_args *args)
{
	sem_unlink(SEM_STICKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_GENER);
	sem_unlink(SEM_MEALS);
	table->sticks = sem_open(SEM_STICKS, O_CREAT, 0644, \
		(unsigned int) args->num);
	if (table->sticks == SEM_FAILED)
		return (clean_table_return(table, "error sem_open", 1));
	table->print = sem_open(SEM_PRINT, O_CREAT, 0644, 1);
	if (table->print == SEM_FAILED)
		return (clean_table_return(table, "error sem_open", 1));
	table->gener = sem_open(SEM_GENER, O_CREAT, 0644, 1);
	if (table->gener == SEM_FAILED)
		return (clean_table_return(table, "error sem_open", 1));
	table->meals = sem_open(SEM_MEALS, O_CREAT, 0644, 0);
	if (table->meals == SEM_FAILED)
		return (clean_table_return(table, "error sem_open", 1));
	if (DEBUG_MOD)
		printf("semaphores are open\n");
	return (0);
}

int	close_semaphores(t_table *table)
{
	if (\
		sem_close(table->sticks) < 0 || \
		sem_close(table->print) < 0 || \
		sem_close(table->gener) < 0 || \
		sem_close(table->meals) < 0)
		return (clean_table_return(table, "error sem_close", 1));
	if (\
		sem_unlink(SEM_STICKS) < 0 || \
		sem_unlink(SEM_PRINT) < 0 || \
		sem_unlink(SEM_GENER) < 0 || \
		sem_unlink(SEM_MEALS) < 0)
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
	if (!SEM_MOD && make_processes(args, &table))
		return (1);
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
