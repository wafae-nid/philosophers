/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnid-hsa <wnid-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:24:31 by wnid-hsa          #+#    #+#             */
/*   Updated: 2025/08/23 01:58:20 by wnid-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	thread_cleaning(t_philo **philos, int i)
{
	t_fork	**fork;
	int		j;

	fork = fork_arry_saving(NULL);
	if (fork)
	{
		j = 1;
		while (j <= philos[1]->data->philo_num)
		{
			pthread_mutex_destroy(&(fork[j]->fork));
			j++;
		}
	}
	j = 1;
	while (j <= i)
	{
		pthread_join(philos[j]->philo_id, NULL);
		pthread_mutex_destroy(&philos[j]->philo_mtx);
		j++;
	}
}

void	destroy_mutexes(t_philo **philos, t_input_data **data)
{
	int		i;
	t_fork	**forks;

	i = 1;
	forks = fork_arry_saving(NULL);
	while (i <= (*data)->philo_num)
	{
		pthread_mutex_destroy(&(philos[i]->philo_mtx));
		i++;
	}
	i = 1;
	while (i <= (*data)->philo_num)
	{
		pthread_mutex_destroy(&(forks[i]->fork));
		i++;
	}
	pthread_mutex_destroy(&(*data)->v_lock);
	pthread_mutex_destroy(&(*data)->print_lock);
}
