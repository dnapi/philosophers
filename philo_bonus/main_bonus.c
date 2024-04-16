/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: apimikov <apimikov@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/16 15:13:14 by apimikov          #+#    #+#             */
/*   Updated: 2024/04/16 17:25:36 by apimikov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

int	make_processes(t_args *args, t_table *table)
{
	int	i;
	int	status;

	i = -1;
	while (++i < args->num)
	{
		table->guests[i]->pos = i + 1;
		table->philos[i] = fork();
		if (table->philos[i] < 0)
			exit(clean_table_return(table, "fork error", 1));
		else if (table->philos[i] == 0)
			routine(table->guests[i]);
	}
	i = -1;
	while (++i < args->num * args->max_eat)
		sem_wait_protected(table->meals);
	if (args->max_eat == -1)
		sem_wait_protected(table->meals);
	i = -1;
	while (++i < args->num)
		kill(table->philos[i], SIGTERM);
	i = -1;
	while (++i < args->num)
		waitpid(table->philos[i], &status, 0);
	return ((status & 0xff00) >> 8);
}

int	open_semaphores(t_table *table, t_args *args)
{
	sem_unlink(SEM_STICKS);
	sem_unlink(SEM_PRINT);
	sem_unlink(SEM_GENER);
	sem_unlink(SEM_MEALS);
	table->sticks = sem_open(SEM_STICKS, O_CREAT, 0644, \
		(unsigned int) args->num);
	if (table->sticks == SEM_FAILED)
		return (clean_table_return(table, "error sem_open", 1));
	table->print = sem_open(SEM_PRINT, O_CREAT, 0644, 1);
	if (table->print == SEM_FAILED)
		return (clean_table_return(table, "error sem_open", 1));
	table->gener = sem_open(SEM_GENER, O_CREAT, 0644, 1);
	if (table->gener == SEM_FAILED)
		return (clean_table_return(table, "error sem_open", 1));
	table->meals = sem_open(SEM_MEALS, O_CREAT, 0644, 0);
	if (table->meals == SEM_FAILED)
		return (clean_table_return(table, "error sem_open", 1));
	if (DEBUG_MOD)
		printf("semaphores are open\n");
	return (0);
}

int	close_semaphores(t_table *table)
{
	if (\
		sem_close(table->sticks) < 0 || \
		sem_close(table->print) < 0 || \
		sem_close(table->gener) < 0 || \
		sem_close(table->meals) < 0)
		return (clean_table_return(table, "error sem_close", 1));
	if (\
		sem_unlink(SEM_STICKS) < 0 || \
		sem_unlink(SEM_PRINT) < 0 || \
		sem_unlink(SEM_GENER) < 0 || \
		sem_unlink(SEM_MEALS) < 0)
		return (clean_table_return(table, "error sem_unlink", 1));
	if (DEBUG_MOD)
		printf("semaphores are closed and unlinked\n");
	return (0);
}

int	do_dinner(t_args *args)
{
	t_table	table;
	int		i;

	(void)i;
	if (init_table(&table, args))
		return (1);
	if (open_semaphores(&table, args))
		return (1);
	if (!SEM_MOD && make_processes(args, &table))
		return (1);
	if (close_semaphores(&table))
		return (1);
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
