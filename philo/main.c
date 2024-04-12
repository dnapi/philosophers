#include "philo.h"

int panic_args(char *msg, int return_value)
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
	if (args->max_eat < 0)
		return (panic_args(ERRMSG_TIMESLE, 1));
	if (argc == 5)
		args->max_eat = -1;
	return (0);
}

void print_debug_mode(int flag, t_args *args)
{
	if (flag)
	{
// this function is to be removed before submission
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

void	set_sticks(t_sage *sage)
{
	int	i;
	int	n;
	int left;

	i = sage->pos - 1;
	n = sage->args->num;
	left = i + (i == 0) * n - 1;
//	if (DEBUG_MOD)
//		printf("My pos=%d, my left fork is %d, and right is %d\n", i, left, i);
	sage->left = sage->table->sticks + left;
	sage->right = sage->table->sticks + i;
}

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

void	printf_mut(t_sage *sage, char *str)
{
	pthread_mutex_lock(sage->table->m_print);
	printf("%lu %d %s\n", get_current_time(), sage->pos, str);
	pthread_mutex_unlock(sage->table->m_print);
}

int	continue_dinner(t_table *table)
{
	int	num_eats;
	int	pasta_flag;

	if (table->args->argc == 5)
		return (1);
	pthread_mutex_lock(table->m_gener);
	num_eats = table->num_eats;
	pasta_flag = table->pasta_flag;
	//printf("pasta=%d\n", pasta_flag);
	pthread_mutex_unlock(table->m_gener);
	if (pasta_flag == 0 || num_eats == 0)
		return (0);
	return (1);
}

void	*routine(void *arg)
{
	t_sage	*sage;
	int	i;

	sage = (t_sage *)arg;
	set_sticks(sage);
	if (sage->pos % 2)
		ft_usleep(1);
	i = -1;
	while (continue_dinner(sage->table))
	{
		//take_meal
		printf("cont = %d\n", continue_dinner(sage->table));
		pthread_mutex_lock(sage->right);
		printf_mut(sage, LOG_FORK);
		pthread_mutex_lock(sage->left);
		printf_mut(sage, LOG_FORK);
		printf_mut(sage, LOG_EAT);
		set_last_meal(sage);
		//sage->last_meal = get_current_time();
		ft_usleep(sage->args->eat);
		reduce_num_eats(sage);
		pthread_mutex_unlock(sage->right);
		pthread_mutex_unlock(sage->left);
		//take_sleep
		if (continue_dinner(sage->table) == 0)
			break ;
		printf_mut(sage, LOG_SLEEP);
		ft_usleep(sage->args->sleep);
		//think_about
		if (!continue_dinner(sage->table))
			break ;
		printf_mut(sage, LOG_THINK);
	}
	return (arg);
}

void	*monitor(void *arg)
{
	t_table	*tab;
	int		i;
	int 	eats_left;
	int		pasta_flag;

	tab = (t_table *)arg;
	pasta_flag = 1;
	//printf("monitor: tab->num_eats=%d\n", tab->num_eats);
	while (tab->num_eats)
	{
		i = -1;
		eats_left = 0;
		pthread_mutex_lock(tab->m_gener);
		//printf("monitor checks\n");
		while (tab->guests[++i])
		{
			eats_left += tab->guests[i]->num_eats;
			if (get_current_time() - \
				tab->guests[i]->last_meal >= tab->args->die)
			{
				tab->pasta_flag = 0;
				pasta_flag = 0;
				printf_mut(tab->guests[i], LOG_DIED);
				break ;
			}
		}
		tab->num_eats = eats_left;
		pthread_mutex_unlock(tab->m_gener);
		if (tab->pasta_flag == 0)
			break ;
		ft_usleep(1);
	}
	return (arg);
}


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
	if (table->sticks)
		free(table->sticks);
	table->sticks = NULL;
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

/*
void init_sage(t_sage *sage, t_table *table)
{
	sage->table = table;
	sage->args = table->args;
	sage->pos = 0;
}
*/

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
		printf("num i=%d is started at %lu\n", i, guests[i]->last_meal);
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
	table->philos = NULL;
	table->guests = NULL;
}

int	init_table(t_table *table, t_args *args)
{
	set_table(table, args);
	//table->sticks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * args->num);
	table->sticks = ft_calloc(args->num + NUM_EXTRA_MUTEX, \
					sizeof(pthread_mutex_t));
	if (!table->sticks)
		return (clean_table_return(table, "malloc err for table->sticks", 1));
	table->philos = (pthread_t *)malloc(sizeof(pthread_t) * \
				(args->num + NUM_EXTRA_THREADS));
	if (!table->philos)
		return (clean_table_return(table, "malloc err for table->philos", 1));
	table->waiter = table->philos + args->num;
	table->guests = (t_sage **)ft_calloc(args->num + 1, sizeof(t_sage *));
	if (!table->guests)
		return (1);
	if (init_guests(table->guests, table))
		return (clean_table_return(table, "malloc err  in init_guests", 1));
	return (0);
}

int	do_dinner(t_args *args)
{
	t_table	table;
	int		i;

	if (init_table(&table, args))
		return (1);
	i = -1;
	while (++i < args->num + NUM_EXTRA_MUTEX)
		pthread_mutex_init(table.sticks + i, NULL);
	table.m_print = table.sticks + args->num;
	table.m_gener = table.sticks + args->num + 1;
	i = -1;
	while (++i < args->num)
	{
		table.guests[i]->pos = i + 1;
		if (pthread_create(table.philos + i, \
				NULL, &routine, table.guests[i]) != 0)
			return (1);
	}
	if (pthread_create(table.waiter, NULL, &monitor, &table) != 0)
		return (1);
	i = -1;
	while (++i < args->num + NUM_EXTRA_THREADS)
	{
		if (pthread_join(table.philos[i], NULL) != 0)
			return (2);
	}
	i = -1;
	while (++i < args->num + NUM_EXTRA_MUTEX)
		pthread_mutex_destroy(table.sticks + i);
	//error check for mutex destroy
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
