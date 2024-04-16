/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/14 06:06:00 by apimikov          #+#    #+#             */
/*   Updated: 2024/04/16 15:32:03 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	make_threads(t_args *args, t_table *table)
{
	int	i;

	i = -1;
	while (++i < args->num)
	{
		table->guests[i]->pos = i + 1;
		if (pthread_create(table->philos + i, \
				NULL, &routine, table->guests[i]) != 0)
			return (clean_table_return(table, "error pthread_create", 1));
	}
	if (pthread_create(table->waiter, NULL, &monitor, table) != 0)
		return (clean_table_return(table, "error pthread_create", 1));
	i = -1;
	while (++i < args->num + NUM_EXTRA_THREADS)
	{
		if (pthread_join(table->philos[i], NULL) != 0)
			return (clean_table_return(table, "error pthread_join", 1));
	}
	return (0);
}

int	do_dinner(t_args *args)
{
	t_table	table;
	int		i;

	if (init_table(&table, args))
		return (1);
	i = -1;
	while (++i < args->num + NUM_EXTRA_MUTEX)
	{
		if (pthread_mutex_init(table.sticks + i, NULL) == -1)
			return (clean_table_return(&table, "error pthread_mutex_init", 1));
	}
	table.m_print = table.sticks + args->num;
	table.m_gener = table.sticks + args->num + 1;
	if (make_threads(args, &table))
		return (1);
	i = -1;
	while (++i < args->num + NUM_EXTRA_MUTEX)
	{
		if (pthread_mutex_destroy(table.sticks + i) == -1)
			return (clean_table_return(&table, "err:pthread_mutex_destr", 1));
	}
	clean_table_return(&table, NULL, 0);
	return (0);
}

int	main(int argc, char *argv[])
{
	t_args	args;

	if (set_args(&args, argc, argv))
		return (1);
	if (argc == 6 && args.max_eat == 0)
		return (0);
	print_debug_mode(DEBUG_MOD, &args);
	if (do_dinner(&args))
		return (1);
	return (0);
}
