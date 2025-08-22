/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnid-hsa <wnid-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 02:34:47 by wnid-hsa          #+#    #+#             */
/*   Updated: 2025/08/22 04:47:34 by wnid-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"


t_input_data	*fill_data_struct(char **argv)
{
	t_input_data	*data;
	int				flag;

	(1 && (data = gc_malloc(sizeof(t_input_data))),(flag = 0));
	if (!data)
		return (NULL);
	else
	{
		(1 && (data->philos_numb = long_atoi(argv[1])),(data->time_tdie = long_atoi(argv[2])));
		(1 && (data->time_teat = long_atoi(argv[3]), data->time_tsleep = long_atoi(argv[4])));
		if(argv[5])
			data->nbr_of_eats = long_atoi(argv[5]);
		else
			data->nbr_of_eats = -1;
		if(pthread_mutex_init(&(data->print_lock), NULL) == -1)
			return(NULL);
		(1 && (data->philos_born = 0), (data->all_full = 0));
		if(pthread_mutex_init(&(data->var_lock), NULL) == -1)
			return(NULL);
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
void	philo_action_loop(t_philo *philo)
{
	while(mutex_var_read(&(philo->data->var_lock), &(philo->data->dinner_is_done)) != 1)
	{
		if(philo->data->nbr_of_eats > 0 && mutex_var_read(&(philo->philo_mutex), &(philo->meals_tracker)) >= philo->data->nbr_of_eats) 
		{
			mutex_var_change(&(philo->philo_mutex), &(philo->philo_full), 1);
			continue;
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
}
void	*philos_actions(void *arg)
{
	t_philo		*philo;
	long		dinner_done;
	
	philo = (t_philo *)arg;
	while (!mutex_var_read(&(philo->data->var_lock), &(philo->data->philos_born)))
		;
	if(philo->data->philos_numb == 1)
	{	
		mutex_printf((philo->data), 5, philo->philo_position);
		usleep((philo)->data->time_tdie );
		return(NULL);
	}
	if(philo->philo_position% 2 != 0)
		small_sleep(philo->data, (philo)->data->time_teat/2);
	philo_action_loop(philo);
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
		mutex_var_change(&(philos[1]->data->var_lock), &(philos[1]->data->dinner_is_done), 1);
}
void *tracker_loop(t_philo **philos, t_helper *helper)
{
	int i;
	
	while (!mutex_var_read(&(philos[1]->data->var_lock), &(philos[1]->data->dinner_is_done)))
    {
        i = 0;
        while (++i <= philos[1]->data->philos_numb)
        {
			if(!helper)
				return(NULL);
            helper->curr_time = time_in_mill();
            helper->last_meal = mutex_var_read(&(philos[i]->philo_mutex), &(philos[i]->last_meal_time));
            helper->death_time =philos[i]->data->time_tdie;
            if ((helper->curr_time - helper->last_meal) > helper->death_time)
            {
				if (!mutex_var_read(&(philos[i]->philo_mutex) ,&(philos[i]->philo_full))) 
				{
                	mutex_printf(philos[i]->data, 4, philos[i]->philo_position);
                	mutex_var_change(&(philos[i]->data->var_lock), &(philos[i]->data->dinner_is_done), 1);
                	return (NULL);
				} 
            }
        }
		are_they_all_full(philos);
    }
	return(NULL);
}
	
void *tracker_function(void *arg)
{
    int i;
    t_philo **philos;
	t_helper *helper;
  
    philos = (t_philo **)arg;
	helper = gc_malloc(sizeof(t_helper));
	return(tracker_loop(philos, helper));
}
void fork_distribution(t_philo **philos, t_fork **forks, int i)
{
	t_input_data	*data;
	int				position;

	data = philos[1]->data;
	if(i%2 == 0)
	{
		philos[i]->left_fork = (forks[i]);
		position = ((i)%(data)->philos_numb)+1;
		philos[i]->right_fork = (forks[position]);
	}
	else
	{
		position = ((i)%((data)->philos_numb))+1;
		philos[i]->left_fork = (forks[position]);
		philos[i]->right_fork = (forks[i]);
	} 
}
t_fork	**fork_arry_saving(t_fork	 **fork)
{
	static t_fork **forks;

	if(fork)
	{
		forks = fork;
		return(forks);
	}
	else
		return(forks);
	
}
t_fork **fork_struct_alloc(t_input_data *data, t_fork **forks)
{
	int i;
	int flag;
	
	if (!forks)
		return(NULL);
	i = 1;
	while(i <= (data)->philos_numb)
	{	
		forks[i] = gc_malloc(sizeof(t_fork));
		if(!forks[i])
			return(NULL);
		if(pthread_mutex_init(&(forks[i]->fork), NULL) == -1)
		{
			while(i >= 1)
			{
				pthread_mutex_destroy(&(forks[i]->fork));
				i--;
			}
			return(NULL);
		}
		forks[i]->fork_nbr = i;
		i++;
	}
	return(forks);
}
void  thread_cleaning(t_philo **philos, int i)
{
	t_fork 	**forks;
	
	forks = fork_arry_saving(NULL);
	while(i >=1 )
	{
		pthread_join(philos[i]->philo_thread_id, NULL);
		pthread_mutex_destroy(&(philos[i]->philo_mutex));
		pthread_mutex_destroy(&(forks[i]->fork));
		i--;
	}
}

t_philo **philo_struct_alloc(t_input_data *data, t_fork **forks, t_philo **philos)
{
	int i;

	i = 1;
	while(i <= (data)->philos_numb)
	{
		philos[i] = gc_malloc(sizeof(t_philo));
		if (!philos[i])
			return (NULL);
		philos[i]->last_meal_time = time_in_mill();
		(1&& (philos[i]->meals_tracker = 0),((philos[i])->data = (data)));
		(1 && (philos[i]->philo_position = i),(fork_distribution(philos, forks, i)));
		philos[i]->philo_full = 0; 
		pthread_mutex_init(&(philos[i]->philo_mutex), NULL);
		if(pthread_mutex_init(&(philos[i]->philo_mutex), NULL))
		{
			thread_cleaning(philos, i);
			return(NULL); 
		}
		if(pthread_create(&(philos[i]->philo_thread_id), NULL, philos_actions, philos[i]) != 0)
		{
			thread_cleaning(philos, i);
				return(NULL);
		}
		i++;
	}
	mutex_var_change(&((data)->var_lock), &((data)->philos_born), 1);
	return(philos);
}
t_philo	**philo_birth(t_input_data	**data)
{
	int	i;
	
	t_philo  **philos;
	t_fork	 **forks;
	long	position;
	
	philos = gc_malloc(((*data)->philos_numb + 1)*sizeof(t_philo*));
	if (!philos)
		return (NULL);
	forks = gc_malloc(((*data)->philos_numb + 1)*sizeof(t_fork*));
	forks=fork_struct_alloc(*data, forks);
	if (!forks)
		return(NULL);
	(void)fork_arry_saving(forks);
	return(philo_struct_alloc(*data, forks, philos));
}

void	destroy_mutexes(t_philo **philos, t_input_data **data)
{
	int i;
	t_fork **forks;
	
	i = 1;
	forks = fork_arry_saving(NULL);
	while(philos[i])
	{
		pthread_mutex_destroy(&(philos[i]->philo_mutex));
		i++;
	}
	i = 1;
	while(forks[i])
	{
		pthread_mutex_destroy(&(forks[i]->fork));
		i++;
	}
	pthread_mutex_destroy(&(*data)->var_lock);
	pthread_mutex_destroy(&(*data)->print_lock);
	
}
void	start_philo_execution(t_input_data	**data)
{
	int	i;
	
	t_philo  	**philos;
	pthread_t	tracker_thread;
	
	philos = philo_birth(data);
	if(!philos)
		return;
	mutex_var_change(&(*data)->var_lock, &(*data)->current_time ,time_in_mill());
	pthread_create(&tracker_thread, NULL, tracker_function, philos);
	i = 1 ;
	while (i <= (*data)->philos_numb)
	{
		pthread_join(philos[i]->philo_thread_id, NULL);
		i++;
	}
	pthread_join(tracker_thread, NULL);
	destroy_mutexes(philos, data);
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
			{
				gc_malloc(0);
				return(0);
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
