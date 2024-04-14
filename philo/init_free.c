/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_free.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alex <alex@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 06:08:25 by apimikov          #+#    #+#             */
/*   Updated: 2024/04/14 06:53:21 by alex             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	clean_guests(t_sage **guests)
{
	int	i;

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

int	clean_table_return(t_table *table, char *msg, int ret)
{
	if (!table)
		return (ret);
	if (table->sticks)
		free(table->sticks);
	table->sticks = NULL;
	if (table->forks)
		free(table->forks);
	table->forks = NULL;
	if (table->philos)
		free(table->philos);
	table->philos = NULL;
	if (table->guests)
		clean_guests(table->guests);
	table->guests = NULL;
	if (msg && *msg)
		printf("%s: %s\n", MSG_NAME, msg);
	return (ret);
}

int	init_guests(t_sage **guests, t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->args->num)
	{
		guests[i] = (t_sage *)malloc(sizeof(t_sage));
		if (!guests[i])
		{
			while (i--)
				free(guests[i]);
			return (1);
		}
		guests[i]->pos = 0;
		guests[i]->num_eats = table->args->max_eat;
		guests[i]->table = table;
		guests[i]->args = table->args;
		guests[i]->last_meal = get_current_time();
	}
	guests[i] = NULL;
	return (0);
}

void	set_table(t_table *table, t_args *args)
{
	table->args = args;
	table->pasta_flag = 1;
	table->num_eats = table->args->max_eat * table->args->num;
	table->sticks = NULL;
	table->forks = NULL;
	table->philos = NULL;
	table->guests = NULL;
}

int	init_table(t_table *table, t_args *args)
{
	set_table(table, args);
	table->sticks = ft_calloc(args->num + NUM_EXTRA_MUTEX, \
					sizeof(pthread_mutex_t));
	if (!table->sticks)
		return (clean_table_return(table, "malloc err for table->sticks", 1));
	table->forks = ft_calloc(args->num, sizeof(int));
	if (!table->forks)
		return (clean_table_return(table, "malloc err for table->fork", 1));
	table->philos = (pthread_t *)malloc(sizeof(pthread_t) * \
				(args->num + NUM_EXTRA_THREADS));
	if (!table->philos)
		return (clean_table_return(table, "malloc err for table->philos", 1));
	table->waiter = table->philos + args->num;
	table->guests = (t_sage **)ft_calloc(args->num + 1, sizeof(t_sage *));
	if (!table->guests)
		return (1);
	if (init_guests(table->guests, table))
		return (clean_table_return(table, "malloc err  in init_guests", 1));
	return (0);
}
