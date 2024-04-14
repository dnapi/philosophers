/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 06:14:56 by apimikov          #+#    #+#             */
/*   Updated: 2024/04/14 12:42:14 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

void	printf_sem(t_sage *sage, char *str)
{
	pthread_mutex_lock(sage->table->m_print);
	if (continue_dinner(sage->table))
		printf("%lu %d %s\n", get_current_time(), sage->pos, str);
	pthread_mutex_unlock(sage->table->m_print);
}
