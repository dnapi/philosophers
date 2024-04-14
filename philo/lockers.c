/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lockers.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 06:12:13 by apimikov          #+#    #+#             */
/*   Updated: 2024/04/14 06:52:44 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	int	n;

	if (sage->pos == 1)
		n = sage->args->num - 1;
	else
		n = sage->pos - 2;
	pthread_mutex_lock(sage->left);
	sage->table->forks[n] = 1;
}

void	unlock_left(t_sage *sage)
{
	int	n;

	if (sage->pos == 1)
		n = sage->args->num - 1;
	else
		n = sage->pos - 2;
	sage->table->forks[n] = 0;
	pthread_mutex_unlock(sage->left);
}
