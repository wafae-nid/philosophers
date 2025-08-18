/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnid-hsa <wnid-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 02:34:47 by wnid-hsa          #+#    #+#             */
/*   Updated: 2025/08/18 12:43:07 by wnid-hsa         ###   ########.fr       */
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
		data->current_time = 0;
		pthread_mutex_init(&(data->var_lock), NULL);
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
	
	philo = (t_philo *)arg;
	while (!philo->data->philos_born)
		;
	if(philo->left_fork->fork_nbr % 2 != 0)
		usleep(((philo)->data->time_tsleep)* 1000);
	pthread_mutex_lock(&(philo->data->var_lock));
	philo->data->current_time =time_in_mill();
	if(philo->data->current_time == -1)
	{
		pthread_mutex_unlock(&(philo->data->var_lock));
		return(NULL); 
	}
	pthread_mutex_unlock(&(philo->data->var_lock));
	while(1)
	{	
		pthread_mutex_lock(&(philo->left_fork->fork));
		printf("philo %d has taken the left fork\n", philo->left_fork->fork_nbr);
		pthread_mutex_lock(&(philo->right_fork->fork));
		printf("philo %d has taken the righet fork\n", philo->left_fork->fork_nbr);
		philo->last_meal_time = time_in_mill();
		philo->meals_tracker = philo->meals_tracker + 1;
		usleep(((philo)->data->time_teat)* 1000);
		printf("philo %d is eating uwu\n", (philo->left_fork->fork_nbr));
		pthread_mutex_unlock(&(philo->left_fork->fork));
		pthread_mutex_unlock(&(philo->right_fork->fork));
		pthread_mutex_lock(&(philo->data->print_lock));
		printf("philo %d is sleeping zzzz\n", (philo->left_fork->fork_nbr));
		usleep(((philo)->data->time_tsleep)* 1000);
		pthread_mutex_unlock(&(philo->data->print_lock));
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
		philos[i]->last_meal_time = 0;
		philos[i]->right_fork = (forks[i]);
		position = (i%((*data)->philos_numb))+1;
		philos[i]->left_fork = (forks[position]);
		philos[i]->meals_tracker = 0;
		(philos[i])->data = (*data);
		pthread_create(&(philos[i]->philo_thread_id), NULL, philos_actions, philos[i]);
		i++;
	}
	(*data)->philos_born = 1;
	return(philos);
}

void	start_philo_execution(t_input_data	**data)
{
	int	i;
	
	t_philo  **philos;
	
	philos = philo_birth(data);
	i = 1 ;

	while (i <= (*data)->philos_numb)
	{
		pthread_join(philos[i]->philo_thread_id, NULL);
		i++;
	}
		
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