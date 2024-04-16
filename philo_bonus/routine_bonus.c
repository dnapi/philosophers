/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:15:38 by apimikov          #+#    #+#             */
/*   Updated: 2024/04/16 17:15:47 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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
	ft_usleep(30);
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
		ft_usleep(50);
	}
	if (sage->args->num > 1 && \
		sage->pos == sage->args->num && \
		sage->args->num % 2)
		ft_usleep(50);
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
