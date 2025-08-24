/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   forks.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnid-hsa <wnid-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:17:07 by wnid-hsa          #+#    #+#             */
/*   Updated: 2025/08/24 03:20:05 by wnid-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	fork_take(t_philo **philos, t_fork **forks, int i)
{
	t_input_data	*data;

	data = philos[1]->data;
	
	if(i == 1)
		philos[i]->left_fork = forks[philos[i]->data->philo_num];
	else
		philos[i]->left_fork = forks[i - 1];
	philos[i]->right_fork = forks[i];
}

t_fork	**fork_arry_saving(t_fork **fork)
{
	static t_fork	**forks;

	if (fork)
	{
		forks = fork;
		return (forks);
	}
	else
		return (forks);
}

void	take_forks(t_philo *philo)
{
	int		philo_id;

	philo_id = philo->philo_position;
	pthread_mutex_lock(&(philo->left_fork->fork));
	mutex_printf(philo->data, 5, philo_id);
	pthread_mutex_lock(&(philo->right_fork->fork));
	mutex_printf(philo->data, 6, philo_id);
}

void	drop_forks(t_philo *philo)
{
	pthread_mutex_unlock(&(philo->left_fork->fork));
	pthread_mutex_unlock(&(philo->right_fork->fork));
}
