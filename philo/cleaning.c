/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleaning.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnid-hsa <wnid-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:24:31 by wnid-hsa          #+#    #+#             */
/*   Updated: 2025/08/22 18:14:43 by wnid-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	thread_cleaning(t_philo **philos, int i)
{
	while (i >= 1)
	{
		pthread_join(philos[i]->philo_id, NULL);
		i--;
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
