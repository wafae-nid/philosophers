/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cust_mtx_func.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnid-hsa <wnid-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/22 17:22:23 by wnid-hsa          #+#    #+#             */
/*   Updated: 2025/08/22 18:08:19 by wnid-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	mutex_printf(t_input_data *data, int flag, int id)
{
	long	time;

	pthread_mutex_lock(&(data->print_lock));
	time = time_in_mill() - mutex_read(&(data->v_lock),
			&(data->current_time));
	if (flag == 1 && !mutex_read(&(data->v_lock), &(data->end)))
		printf("%lu %d is eating\n", time, id);
	else if (flag == 2 && !mutex_read(&(data->v_lock), &(data->end)))
		printf("%lu %d is sleeping\n", time, id);
	else if (flag == 3 && !mutex_read(&(data->v_lock), &(data->end)))
		printf("%lu %d is thinking\n", time, id);
	else if (flag == 4)
	{
		printf("%lu %d is dead\n", time, id);
		mutex_var_change(&data->v_lock, &data->end, 1);
	}
	else if (flag == 5 && !mutex_read(&(data->v_lock), &(data->end)))
		printf("%lu %d has taken the first fork\n", time, id);
	else if (flag == 6 && !mutex_read(&(data->v_lock), &(data->end)))
		printf("%lu %d has taken the second fork\n", time, id);
	else if (flag == 7 && !mutex_read(&(data->v_lock), &(data->end)))
		printf("%lu %d is thinking\n", time, id);
	pthread_mutex_unlock(&(data->print_lock));
}

void	mutex_var_change(pthread_mutex_t *v_lock, long *var, long new_value)
{
	pthread_mutex_lock(v_lock);
	*var = new_value;
	pthread_mutex_unlock(v_lock);
}

long	mutex_read(pthread_mutex_t *v_lock, long *var)
{
	long	value;

	pthread_mutex_lock(v_lock);
	value = *var;
	pthread_mutex_unlock(v_lock);
	return (value);
}

long	mutex_var_change_plus(pthread_mutex_t *v_lock, long *var)
{
	pthread_mutex_lock(v_lock);
	*var = *var + 1;
	pthread_mutex_unlock(v_lock);
	return (*var);
}
