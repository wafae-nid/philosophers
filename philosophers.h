/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnid-hsa <wnid-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 02:40:05 by wnid-hsa          #+#    #+#             */
/*   Updated: 2025/08/18 12:39:30 by wnid-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

#include <stdio.h>
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#include <pthread.h>
#include <sys/time.h>

typedef struct s_input_data
{
	long			philos_numb;
	long			time_tdie;
	long			time_teat;
	long			time_tsleep;
	long			nbr_of_eats;
	pthread_mutex_t print_lock;
	int				philos_born;
	int				current_time;
	pthread_mutex_t	var_lock;
}			t_input_data;

typedef struct s_fork
{
	pthread_mutex_t fork;
	int				fork_nbr;
}	t_fork;

typedef struct s_philo
{
	pthread_t 		philo_thread_id;
	long			last_meal_time;
	t_fork			*left_fork;
	t_fork			*right_fork;
	long			meals_tracker;
	t_input_data 	*data;
}	t_philo;	

int		ft_is_a_numb(char c);
int 	all_valid_chars(int argc, char **argv);
long	long_atoi(const char *str);
int		all_checkings(int argc, char **argv);
void	print_input_error(int flag);

#endif