/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:15:38 by apimikov          #+#    #+#             */
/*   Updated: 2024/04/21 05:09:43 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	*monitor(void *arg)
{
//	t_sage	*sage;
//	int		i;

//	sage = (t_sage *)arg;
	//usleep(2000000);
	return (arg);
}

int	routine(void *arg)
{
	(void)arg;
	/*
	t_sage	*sage;

	sage = (t_sage *)arg;
	sem_wait_protected(sage->table->gener);
	if (pthread_create(&(sage->ghost), NULL, &monitor, sage) != 0)
		exit (clean_table_return(sage->table, "error pthread_create", \
			EXIT_FAIL));

//	printf_sem(sage, "about to sleep");
	//ft_usleep(2000);
	if (pthread_join(sage->ghost, NULL) != 0)
		exit (clean_table_return(sage->table, "err pthread_join", EXIT_FAIL));
	sem_post_protected(sage->table->gener);
*/
	exit (EXIT_SUCCESS);
}
