/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_utils_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:16:44 by apimikov          #+#    #+#             */
/*   Updated: 2024/04/16 17:22:39 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	print_return(char *msg, int ret)
{
	if (msg && *msg)
	{
		printf("%s: %s\n", MSG_NAME, msg);
	}
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
