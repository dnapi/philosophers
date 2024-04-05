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

int	check_args(int argc, char **argv)
{
	int		i;
	char	*s;

	if (argc < 5 || 6 < argc)
	{
		printf("phil: wrong number of arguments\n");
		return (1);
	}
	i = 0;
	while (++i < argc)
	{
		s = argv[i];
		while (*s)
		{
//			printf("%c =%d, 0=%d, 9=%d\n", *s, (int)*s, (int)'0', (int)'9');
			if (*s < '0' || '9' < *s)
			{
				printf("phil: arguments must be integers \n");
				return (1);
			}
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
	return (0);
}

int	main(int argc, char *argv[])
{
	t_philo	philo;

	if (set_phil(&philo, argc, argv))
		return (1);
	printf("args are fine\n");
	printf("num of philo=%d\n", philo.num);
	return (0);
}
