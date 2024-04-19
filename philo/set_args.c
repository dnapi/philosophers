/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_args.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 06:45:53 by apimikov          #+#    #+#             */
/*   Updated: 2024/04/17 15:45:25 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

size_t	ft_strlen(const char *s)
{
	char	*pnt;

	pnt = (char *)s;
	while (*pnt)
		pnt++;
	return (pnt - s);
}

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
		if (ft_strlen(s) > 7)
			return (panic_args(ERRMSG_INT, 1));
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
	if (args->num < 1 || args->num > 200)
		return (panic_args(ERRMSG_NUMPHIL, 1));
	args->die = ft_atoil(argv[2]);
	if (args->die < 1 || args->die > MAX_TIME)
		return (panic_args(ERRMSG_TIMEDIE, 1));
	args->eat = ft_atoil(argv[3]);
	if (args->eat < 1 || args->eat > MAX_TIME)
		return (panic_args(ERRMSG_TIMEEAT, 1));
	args->sleep = ft_atoil(argv[4]);
	if (args->sleep < 1 || args->sleep > MAX_TIME)
		return (panic_args(ERRMSG_TIMESLE, 1));
	if (argc == 6)
		args->max_eat = ft_atoil(argv[5]);
	if (argc == 6 && (args->max_eat <= 0 || args->max_eat > MAX_EAT_NUM))
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
