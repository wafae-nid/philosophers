/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnid-hsa <wnid-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 02:34:47 by wnid-hsa          #+#    #+#             */
/*   Updated: 2025/08/21 03:34:05 by wnid-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


t_input_data	*fill_data_struct(char **argv)
{
	t_input_data	*data;

	data = malloc(sizeof(t_input_data));
	if (!data)
		return (NULL);
	else
	{
		data->philos_numb = long_atoi(argv[1]);
		data->time_tdie = long_atoi(argv[2]);
		data->time_teat = long_atoi(argv[3]);
		data->time_tsleep = long_atoi(argv[4]);
		if(argv[5])
			data->nbr_of_eats = long_atoi(argv[5]);
		else
			data->nbr_of_eats = -1;
		pthread_mutex_init(&(data->print_lock), NULL);
		data->philos_born = 0;
		data->all_full = 0;
		pthread_mutex_init(&(data->var_lock), NULL);
		data->dinner_is_done = 0;
		return (data);
	}
}

long	time_in_mill(void)
{
	struct timeval time;
	
	long	actual_time;

	gettimeofday(&time, NULL);
	actual_time = (time.tv_sec *1000) + (time.tv_usec /1000);
	return(actual_time);
}
void	*philos_actions(void *arg)
{
	
	t_philo		*philo;
	long		dinner_done;
	
	philo = (t_philo *)arg;
	while (!mutex_var_read(&(philo->data->var_lock), &(philo->data->philos_born)))
		;
	if(philo->philo_position% 2 != 0)
		small_sleep(philo->data, (philo)->data->time_teat/2);
	while(mutex_var_read(&(philo->data->var_lock), &(philo->data->dinner_is_done)) != 1)
	{
		if(philo->data->nbr_of_eats > 0 && mutex_var_read(&(philo->philo_mutex), &(philo->meals_tracker)) == philo->data->nbr_of_eats )
		{
			mutex_var_change(&(philo->philo_mutex), &(philo->philo_full), 1);
			break;
		}
		take_forks(philo);
		mutex_var_change(&(philo->philo_mutex), &(philo->last_meal_time), time_in_mill());
		mutex_printf((philo->data), 1, philo->philo_position);
		small_sleep(philo->data, (philo)->data->time_teat);
		mutex_var_change_plus(&(philo->philo_mutex), &(philo->meals_tracker));
		drop_forks(philo);
		mutex_printf((philo->data), 2, philo->philo_position);
		small_sleep(philo->data, (philo)->data->time_tsleep);

	}
	return(NULL);
}
void	are_they_all_full( t_philo **philos)
{
	int i;
	int all_satisfied;

	i = 1;
	all_satisfied = 1;
	while(i<= philos[1]->data->philos_numb)
	{
	 	if (mutex_var_read(&(philos[i])->philo_mutex, &(philos[i]->philo_full)) != 1)
			all_satisfied = 0;
		i++;
	}
	if(all_satisfied == 1)
	{
		mutex_var_change(&(philos[1]->data->var_lock), &(philos[1]->data->all_full), 1);
		mutex_var_change(&(philos[i]->data->var_lock), &(philos[i]->data->dinner_is_done), 1);
	}
		
}
void *tracker_function(void *arg)
{
    int i;
    t_philo **philos;
    long curr_time;
    
    long philo_last_meal;
    long philo_death_time;
    int meals_eaten;
    philos = (t_philo **)arg;
    
    while (!mutex_var_read(&(philos[1]->data->var_lock), &(philos[1]->data->dinner_is_done)))
    {
        i = 1;
        while (i <= philos[1]->data->philos_numb)
        {
            curr_time = time_in_mill();
            philo_last_meal = mutex_var_read(&(philos[i]->philo_mutex), &(philos[i]->last_meal_time));
            philo_death_time =philos[i]->data->time_tdie;
            if ((curr_time - philo_last_meal) >= philo_death_time)
            {
                mutex_printf(philos[i]->data, 4, philos[i]->philo_position);
                mutex_var_change(&(philos[i]->data->var_lock), &(philos[i]->data->dinner_is_done), 1);
                return (NULL);
            }
            i++;
        }
		are_they_all_full(philos);
    }
	return(NULL);
}

t_philo	**philo_birth(t_input_data	**data)
{
	int	i;
	
	t_philo  **philos;
	t_fork	 **forks;
	long	position;
	
	philos = malloc(((*data)->philos_numb + 1)*sizeof(t_philo*));
	if (!philos)
		return (NULL);
	forks = malloc(((*data)->philos_numb + 1)*sizeof(t_fork*));
	if (!forks)
		return(NULL);
	i = 1;
	while(i <= (*data)->philos_numb)
	{	
		forks[i] = malloc(sizeof(t_fork));
		if(!forks[i])
			return(NULL);
		pthread_mutex_init(&(forks[i]->fork), NULL);
		forks[i]->fork_nbr = i;
		i++;
	} 
	i = 1;
	while(i <= (*data)->philos_numb)
	{
		philos[i] = malloc(sizeof(t_philo));
		if (!philos[i])
			return (NULL);
		philos[i]->last_meal_time = time_in_mill();
			
		philos[i]->meals_tracker = 0;
		(philos[i])->data = (*data);
		philos[i]->philo_position = i;
		if(i%2 == 0)
		{
			philos[i]->left_fork = (forks[i]);
			position = ((i)%((*data)->philos_numb))+1;
			philos[i]->right_fork = (forks[position]);
		}
		else
		{
			position = ((i)%((*data)->philos_numb))+1;
			philos[i]->left_fork = (forks[position]);
			philos[i]->right_fork = (forks[i]);
		} 
		philos[i]->philo_full = 0; 
		pthread_mutex_init(&(philos[i]->philo_mutex), NULL);
		pthread_create(&(philos[i]->philo_thread_id), NULL, philos_actions, philos[i]);
		i++;
	}
	mutex_var_change(&((*data)->var_lock), &((*data)->philos_born), 1);
	return(philos);
}


void	start_philo_execution(t_input_data	**data)
{
	int	i;
	
	t_philo  	**philos;
	pthread_t	tracker_thread;
	
	
	philos = philo_birth(data);
	mutex_var_change(&(*data)->var_lock, &(*data)->current_time ,time_in_mill());
	pthread_create(&tracker_thread, NULL, tracker_function, philos);
	i = 1 ;

	while (i <= (*data)->philos_numb)
	{
		pthread_join(philos[i]->philo_thread_id, NULL);
		i++;
	}
	pthread_join(tracker_thread, NULL);
}
int	main(int argc, char **argv)
{
	t_input_data	*data;
	
	if( argc == 5 || argc == 6)
	{
		if (all_checkings(argc, argv))
		{
			data = fill_data_struct(argv);
			if(!data)
				return(0);
			start_philo_execution(&data);
		}
		else
			print_input_error(1);
	}
	else
		print_input_error(0);
}
