#include <stdio.h>
#include "philo.h"

/*
typedef struct s_philo
{
	int num; 
	int die;
	int eat;
	int sleep;
	int	max_eat;
}	t_philo;
*/

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
//			printf("%c =%d, 0=%d, 9=%d\n", *s, (int)*s, (int)'0', (int)'9');
			if (*s < '0' || '9' < *s)
				return (panic_args(ERRMSG_INT, 1));
			s++;
		}
	}
	return (0);
}

int	set_phil(t_philo *philo, int argc, char **argv)
{
	if (check_args(argc, argv))
		return (1);
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
	philo->max_eat = ft_atoil(argv[5]);
	return (0);
}

void print_debug_mode(int flag, t_philo *philo)
{
	if (flag)
	{

		printf("-----------   DEBUG MODE  -------- \n");
		printf("args are fine\n");
		printf("num=%d\n", philo->num);
		printf("die=%d\n", philo->die);
		printf("eat=%d\n", philo->eat);
		printf("sleep=%d\n", philo->sleep);
		printf("max_eat=%d\n", philo->max_eat);
		printf("-----------   END OF DEBUG  -------- \n");
	}
}

int	main(int argc, char *argv[])
{
	t_philo	philo;

	if (set_phil(&philo, argc, argv))
		return (1);
	print_debug_mode(DEBUG_MOD, &philo);
	return (0);
}
