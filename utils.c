/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnid-hsa <wnid-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 20:15:38 by wnid-hsa          #+#    #+#             */
/*   Updated: 2025/08/21 20:36:09 by wnid-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


void take_forks(t_philo *philo)
{
	int		philo_id;
	philo_id = philo->philo_position;
	
    pthread_mutex_lock(&(philo->left_fork->fork));
    mutex_printf(philo->data, 5, philo_id); 
    pthread_mutex_lock(&(philo->right_fork->fork));
    mutex_printf(philo->data, 6, philo_id);
}

void drop_forks(t_philo *philo)
{
    pthread_mutex_unlock(&(philo->left_fork->fork));
    pthread_mutex_unlock(&(philo->right_fork->fork));
}
void	print_input_error(int flag)
{
	if (flag == 0)
		printf("arguments should be 5 or optionally 6 ....try again :) \n");
	else 
		printf("arguments should be all positive numbers and less than int max .... try again:) \n");
	exit(EXIT_FAILURE);
}

void	mutex_printf(t_input_data *data, int flag, int id)
{
	long	time;

	pthread_mutex_lock(&(data->print_lock));
	time = time_in_mill() - mutex_var_read(&(data->var_lock), &(data->current_time));
	
	if(flag == 1 && !mutex_var_read(&(data->var_lock),&(data->dinner_is_done)))
		printf("%lu %d is eating\n", time, id);
	else if(flag == 2 && !mutex_var_read(&(data->var_lock),&(data->dinner_is_done)))
		printf("%lu %d is sleeping\n", time,id);
	else if(flag == 3 && !mutex_var_read(&(data->var_lock),&(data->dinner_is_done)))	
		printf("%lu %d is thinking\n", time,id);
	else if(flag == 4) {
		printf("%lu %d is dead\n", time,id);
		mutex_var_change(&data->var_lock, &data->dinner_is_done, 1);	
	}
	else if(flag == 5 && !mutex_var_read(&(data->var_lock),&(data->dinner_is_done)))
		printf("%lu %d has taken the first fork\n", time,id);
	else if(flag == 6 && !mutex_var_read(&(data->var_lock),&(data->dinner_is_done)))
		printf("%lu %d has taken the second fork\n", time,id);
	else if(flag == 7)
		printf("philosophers are full\n");
	pthread_mutex_unlock(&(data->print_lock));
}

void	mutex_var_change(pthread_mutex_t *var_lock, long *var, long new_value)
{
	pthread_mutex_lock(var_lock);
	*var = new_value;
	pthread_mutex_unlock(var_lock);
}
long	mutex_var_read(pthread_mutex_t *var_lock, long *var)
{
	long value;
	
	pthread_mutex_lock(var_lock);
	value = *var;
	pthread_mutex_unlock(var_lock);
	return(value);
}
long	mutex_var_change_plus(pthread_mutex_t *var_lock, long *var)
{
	pthread_mutex_lock(var_lock);
	*var = *var + 1;
	pthread_mutex_unlock(var_lock);
	return(*var);
}

void small_sleep(t_input_data *data, long time_ms)
{
    long start = time_in_mill();
    while (time_in_mill() - start < time_ms)
    {
        if (mutex_var_read(&data->var_lock, &data->dinner_is_done))
            break;
        usleep(100); 
    }
}
void print_time(t_input_data *data, long time)
{
	pthread_mutex_lock(&(data->print_lock));

	printf("%lu\n", time);
	pthread_mutex_unlock(&(data->print_lock));
}
