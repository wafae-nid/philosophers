/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnid-hsa <wnid-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 02:34:47 by wnid-hsa          #+#    #+#             */
/*   Updated: 2025/08/23 23:13:18 by wnid-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	philo_action_loop(t_philo *philo)
{
	while (mutex_read(&(philo->data->v_lock),
			&(philo->data->end)) != 1)
	{
		if (philo->data->meals_nbr > 0
			&& mutex_read(&(philo->philo_mtx),
				&(philo->meals_tracker)) >= philo->data->meals_nbr)
		{
			mutex_var_change(&(philo->philo_mtx), &(philo->full), 1);
			continue ;
		}
		take_forks(philo);
		mutex_var_change(&(philo->philo_mtx),
			&(philo->last_meal_time), time_in_mill());
		mutex_printf((philo->data), 1, philo->philo_position);
		small_sleep(philo->data, (philo)->data->t_eat);
		mutex_var_change_plus(&(philo->philo_mtx), &(philo->meals_tracker));
		drop_forks(philo);
		mutex_printf((philo->data), 2, philo->philo_position);
		small_sleep(philo->data, (philo)->data->t_sleep);
		mutex_printf((philo->data), 7, philo->philo_position);
	}
}

void	*actions(void *arg)
{
	t_philo		*philo;

	philo = (t_philo *)arg;
	while (!mutex_read(&(philo->data->v_lock), &(philo->data->philos_born)))
		;
	if (philo->data->philo_num == 1)
	{
		mutex_printf((philo->data), 5, philo->philo_position);
		usleep((philo)->data->t_die);
		return (NULL);
	}
	else if (philo->philo_position % 2 != 0)
		small_sleep(philo->data, (philo)->data->t_eat / 2);
	philo_action_loop(philo);
	return (NULL);
}

void	*tracker_loop(t_philo **philos, t_helper *helper)
{
	int		i;

	while (!mutex_read(&(philos[1]->data->v_lock), &(philos[1]->data->end)))
	{
		i = 0;
		while (++i <= philos[1]->data->philo_num)
		{
			helper->curr_time = time_in_mill();
			helper->last_meal = mutex_read(&(philos[i]->philo_mtx),
					&(philos[i]->last_meal_time));
			helper->death_time = philos[i]->data->t_die;
			if ((helper->curr_time - helper->last_meal) > helper->death_time)
			{
				if (!mutex_read(&(philos[i]->philo_mtx), &(philos[i]->full)))
				{
					mutex_printf(philos[i]->data, 4, philos[i]->philo_position);
					mutex_var_change(&(philos[i]->data->v_lock),
						&(philos[i]->data->end), 1);
					return (NULL);
				}
			}
		}
		are_they_all_full(philos);
	}
	return (NULL);
}

void	*tracker_function(void *arg)
{
	t_philo		**philos;
	t_helper	*helper;

	philos = (t_philo **)arg;
	helper = gc_malloc(sizeof(t_helper));
	if (!helper)
		return (NULL);
	return (tracker_loop(philos, helper));
}

int	main(int argc, char **argv)
{
	t_input_data	*data;

	if (argc == 5 || argc == 6)
	{
		if (all_checkings(argc, argv))
		{
			data = fill_data_struct(argv);
			if (!data)
			{
				gc_malloc(0);
				return (1);
			}
			start_philo_execution(&data);
			gc_malloc(0);
		}
		else
			print_input_error(1);
	}
	else
		print_input_error(0);
}
