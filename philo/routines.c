/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 06:10:18 by apimikov          #+#    #+#             */
/*   Updated: 2024/04/14 12:42:14 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_meal(t_sage *sage)
{
	//pthread_mutex_lock(sage->right);
	lock_right(sage);
	printf_sem(sage, LOG_FORK);
	if (sage->args->num > 1)
	{
		lock_left(sage);
//		pthread_mutex_lock(sage->left);
		printf_sem(sage, LOG_FORK);
	}
	else
		return (1);
	printf_sem(sage, LOG_EAT);
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

void	set_sticks(t_sage *sage)
{
	int	i;
	int	n;
	int	left;

	i = sage->pos - 1;
	n = sage->args->num;
	left = i + (i == 0) * n - 1;
	sage->left = sage->table->sticks + left;
	sage->right = sage->table->sticks + i;
}

void	*routine(void *arg)
{
	t_sage	*sage;
	int		i;

	sage = (t_sage *)arg;
	set_sticks(sage);
	if (sage->pos % 2)
		ft_usleep(1);
	i = -1;
	while (continue_dinner(sage->table))
	{
		if (take_meal(sage) || continue_dinner(sage->table) == 0)
			break ;
		printf_sem(sage, LOG_SLEEP);
		ft_usleep(sage->args->sleep);
		if (!continue_dinner(sage->table))
			break ;
		printf_sem(sage, LOG_THINK);
	}
	return (arg);
}

int	set_flags(t_table *tab)
{
	int	pasta_flag;
	int	i;

	pasta_flag = 1;
	i = -1;
	tab->num_eats = 0;
	while (tab->guests[++i])
	{
		tab->num_eats += tab->guests[i]->num_eats;
		if (get_current_time() - \
			tab->guests[i]->last_meal >= (size_t)tab->args->die)
		{
			tab->pasta_flag = 0;
			pasta_flag = 0;
			printf("%lu %d %s\n", get_current_time(),
				tab->guests[i]->pos, LOG_DIED);
			if (DEBUG_MOD)
				printf("duration=%lu\n", get_current_time() - tab->start_time);
			break ;
		}
	}
	return (pasta_flag);
}

void	*monitor(void *arg)
{
	t_table	*tab;
	int		pasta_flag;

	tab = (t_table *)arg;
	pasta_flag = 1;
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
	return (arg);
}
