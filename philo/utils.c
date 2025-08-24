/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnid-hsa <wnid-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 20:15:38 by wnid-hsa          #+#    #+#             */
/*   Updated: 2025/08/23 19:03:48 by wnid-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	are_they_all_full(t_philo **philos)
{
	int		i;
	int		all_satisfied;

	i = 1;
	all_satisfied = 1;
	while (i <= philos[1]->data->philo_num)
	{
		if (mutex_read(&(philos[i])->philo_mtx,
				&(philos[i]->full)) != 1)
			all_satisfied = 0;
		i++;
	}
	if (all_satisfied == 1)
		mutex_var_change(&(philos[1]->data->v_lock),
			&(philos[1]->data->end), 1);
}

long	time_in_mill(void)
{
	struct timeval	time;
	long			actual_time;

	gettimeofday(&time, NULL);
	actual_time = (time.tv_sec * 1000) + (time.tv_usec / 1000);
	return (actual_time);
}

void	print_input_error(int flag)
{
	if (flag == 0)
		printf("arguments should be 5 or optionally 6.\n");
	else
		printf("only positive numbers and less than int max allowed.\n");
}

void	small_sleep(t_input_data *data, long time_ms)
{
	long	start;

	start = time_in_mill();
	while (time_in_mill() - start < time_ms)
	{
		if (mutex_read(&data->v_lock, &data->end))
			break ;
		usleep(100);
	}
}
