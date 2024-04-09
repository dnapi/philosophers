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

int	set_phil(t_args *philo, int argc, char **argv)
{
	if (check_args(argc, argv))
		return (1);
	philo->argc = argc;
	philo->num = ft_atoil(argv[1]);
	if (philo->num < 1)
		return (panic_args(ERRMSG_NUMPHIL, 1));
	philo->die = ft_atoil(argv[2]);
	if (philo->die < 1)
		return (panic_args(ERRMSG_TIMEDIE, 1));
	philo->eat = ft_atoil(argv[3]);
	if (philo->eat < 1)
		return (panic_args(ERRMSG_TIMEEAT, 1));
	philo->sleep = ft_atoil(argv[4]);
	if (philo->sleep < 1)
		return (panic_args(ERRMSG_TIMESLE, 1));
	if (argc == 6)
		philo->max_eat = ft_atoil(argv[5]);
	if (philo->max_eat < 0)
		return (panic_args(ERRMSG_TIMESLE, 1));
	if (argc == 5)
		philo->max_eat = 0;
	return (0);
}

void print_debug_mode(int flag, t_args *philo)
{
	if (flag)
	{
// this function is to be removed before submission
		printf("-----------   DEBUG MODE  -------- \n");
		printf("args are fine\n");
		printf("argc=%d\n", philo->argc);
		printf("num=%d\n", philo->num);
		printf("die=%d\n", philo->die);
		printf("eat=%d\n", philo->eat);
		printf("sleep=%d\n", philo->sleep);
		printf("max_eat=%d\n", philo->max_eat);
		printf("-----------   END OF DEBUG  -------- \n");
	}
}

void	*routine(void *arg)
{
	//sleep(*(int*)num);
	t_table	*table;
	int			pos;

	table = (t_table*)arg;
	pos = table->guests->pos;
	pthread_mutex_lock(sage->table->sticks + 0);
	printf("Hi my index is %d\n", pos);
	sleep(1);
	pthread_mutex_unlock(sage->table->sticks + 0);
	return (arg);
}



void	clean_guests(t_sage **guests)
{
	int i;

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

int clean_table_return(t_table *table, int ret)
{
//	int	i;
	if (table->sticks)
		free(table->sticks);
	table->sticks = NULL;
	if (table->sages)
		free(table->sages);
	table->sages = NULL;
	if (table->ind)
		free(table->ind);
	table->ind = NULL;
	if (table->guests)
	{
		clean_guests(table->guests);
		table->guests = NULL;
	}
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
		guests[i] = (t_sage*)malloc(sizeof(t_sage));
		if (!guests[i])
		{
			while (i--)
				free(guests[i]);
			return (1);
		}
		guests[i]->pos = 0;
	}
}

void	set_table(t_table *table)
{
	table->sticks = NULL;
	table->sages = NULL;
	table->ind = NULL;
	table->guests = NULL;
}

int	init_table(t_table *table, t_args *args)
{
	set_table(table);
	table->guests = (t_sage **)ft_calloc(args->num + 2, sizeof(t_sage*));
	if (!table->guests)
		return (1);
	if (init_guests(table->guests, table))
		return (clean_table_return(table, 1));
	table->sticks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * args->num);
	if (!table->sticks)
		return (clean_table_return(table, 1));
	table->sages = (pthread_t *)malloc(sizeof(pthread_t) * (args->num + 1));
	if (!table->sages)
		return (clean_table_return(table, 1));
	table->ind = (int *)malloc(sizeof(int) * (args->num + 1));
	if (!table->ind)
		return (clean_table_return(table, 1));
	return (0);
}

int	do_dinner(t_args *args)
{
	t_table	table;
	int i;

	if (init_table(&table, args))
		return (1);
	i = -1;
	while (++i < args->num + 1)
		pthread_mutex_init(table.sticks + i, NULL);
	//error check for mutex init
	i = -1;
	while (++i < args->num + 1)
	{
		table.guests[i].pos = i;
		if (pthread_create(table.sages + i, NULL, &routine, &table) != 0)
			return (1);
		printf("thread i=%d has started\n", i);
	}
	i = -1;
	while (++i < args->num + 1)
	{
		if (pthread_join(table.sages[i], NULL) != 0)
			return (2);
	}
	i = -1;
	while (++i < args->num + 1)
		pthread_mutex_destroy(table.sticks + i);
	//error check for mutex destroy
	clean_table_return(&table, 0);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_args	args;

	if (set_phil(&args, argc, argv))
		return (1);
	if (argc == 6 && args.max_eat == 0)
		return (0);
	print_debug_mode(DEBUG_MOD, &args);
	do_dinner(&args);
	return (0);
}
