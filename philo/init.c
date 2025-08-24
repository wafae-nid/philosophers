/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnid-hsa <wnid-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:08:36 by wnid-hsa          #+#    #+#             */
/*   Updated: 2025/08/24 03:52:23 by wnid-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_input_data	*fill_data_struct(char **argv)
{
	t_input_data	*data;
	int				flag;

	(1 && (data = gc_malloc(sizeof(t_input_data))), (flag = 0));
	if (!data)
		return (NULL);
	else
	{
		(1 && (data->philo_num = atol(argv[1])), (data->t_die = atol(argv[2])));
		(1 && (data->t_eat = atol(argv[3]), data->t_sleep = atol(argv[4])));
		data->current_time = time_in_mill();
		if (argv[5])
			data->meals_nbr = atol(argv[5]);
		else
			data->meals_nbr = -1;
		if (pthread_mutex_init(&(data->print_lock), NULL) != 0)
			return (NULL);
		(1 && (data->philos_born = 0), (data->all_full = 0));
		if (pthread_mutex_init(&(data->v_lock), NULL) != 0)
			return (pthread_mutex_destroy(&data->print_lock), NULL);
		data->end = 0;
		return (data);
	}
}

t_fork	**fork_struct_alloc(t_input_data *data, t_fork **forks)
{
	int		i;

	if (!forks)
		return (NULL);
	i = 1;
	while (i <= (data)->philo_num)
	{
		forks[i] = gc_malloc(sizeof(t_fork));
		if (!forks[i])
			return (NULL);
		if (pthread_mutex_init(&(forks[i]->fork), NULL) != 0)
		{
			while (i >= 1)
			{
				pthread_mutex_destroy(&(forks[i]->fork));
				i--;
			}
			return (NULL);
		}
		forks[i]->fork_nbr = i;
		i++;
	}
	return (forks);
}

t_philo	**philo_struct_alloc(t_input_data *data, t_fork **forks,
	t_philo **philos, int i)
{
	while (++i <= (data)->philo_num)
	{
		philos[i] = gc_malloc(sizeof(t_philo));
		if (!philos[i])
			return (NULL);
		philos[i]->last_meal_time = time_in_mill();
		(1 && (philos[i]->meals_tracker = 0), ((philos[i])->data = (data)));
		(1 && (philos[i]->philo_position = i), (fork_take(philos, forks, i)));
		philos[i]->full = 0;
		if (pthread_mutex_init(&(philos[i]->philo_mtx), NULL))
		{
			thread_cleaning(philos, i);
			return (NULL);
		}
		if (pthread_create(&(philos[i]->philo_id), NULL, actions, philos[i]))
		{
			mutex_var_change(&(philos[1]->data)->v_lock, &((philos[1]->data)->philos_born), 1);
			thread_cleaning(philos, i);
			return (NULL);
		}
	}
	mutex_var_change(&((data)->v_lock), &((data)->philos_born), 1);
	return (philos);
}

t_philo	**philo_birth(t_input_data	**data)
{
	t_philo	**philos;
	t_fork	**forks;
	
	philos = gc_malloc(((*data)->philo_num + 1) * sizeof(t_philo *));
	if (!philos)
		return (NULL);
	forks = gc_malloc(((*data)->philo_num + 1) * sizeof(t_fork *));
	forks = fork_struct_alloc(*data, forks);
	if (!forks)
		return (NULL);
	fork_arry_saving(forks);
	return (philo_struct_alloc(*data, forks, philos, 0));
}

void	start_philo_execution(t_input_data	**data)
{
	int			i;
	t_philo		**philos;
	pthread_t	tracker_thread;

	philos = philo_birth(data);
	if (!philos)
		return ;
	mutex_var_change(&(*data)->v_lock, &(*data)->current_time, time_in_mill());
	pthread_create(&tracker_thread, NULL, tracker_function, philos);
	i = 1 ;
	while (i <= (*data)->philo_num)
		pthread_join(philos[i++]->philo_id, NULL);
	pthread_join(tracker_thread, NULL);
	if ((*data)->philo_num > 1)
		destroy_mutexes(philos, data);
	else
		pthread_mutex_destroy(&philos[1]->left_fork->fork);
}
