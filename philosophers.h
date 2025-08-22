/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnid-hsa <wnid-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 02:40:05 by wnid-hsa          #+#    #+#             */
/*   Updated: 2025/08/22 02:42:04 by wnid-hsa         ###   ########.fr       */
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
#include <stdbool.h>

typedef struct s_input_data
{
	long			philos_numb;
	long			time_tdie;
	long			time_teat;
	long			time_tsleep;
	long			nbr_of_eats;
	pthread_mutex_t print_lock;
	long			philos_born;
	long			dinner_is_done;
	long			current_time;
	long			all_full;
	pthread_mutex_t	var_lock;
	
}			t_input_data;

typedef struct s_fork
{
	pthread_mutex_t fork;
	long				fork_nbr;
}	t_fork;

typedef struct s_philo
{
	pthread_t 		philo_thread_id;
	long			last_meal_time;
	t_fork			*left_fork;
	t_fork			*right_fork;
	long			meals_tracker;
	int				philo_position;
	long			philo_full;
	pthread_mutex_t philo_mutex;
	long			thread_fail;
	t_input_data 	*data;
}	t_philo;

typedef struct s_helper
{
	long curr_time;
    long last_meal;
    long death_time;
    int meals_eaten;
}	t_helper;

typedef struct s_trash
{
	void					*point;
	struct s_trash	*next;
}					t_trash;

int		ft_is_a_numb(char c);
int 	all_valid_chars(int argc, char **argv);
long	long_atoi(const char *str);
int		all_checkings(int argc, char **argv);
void	print_input_error(int flag);
void	mutex_printf(t_input_data *data, int flag, int id);
void	mutex_var_change(pthread_mutex_t *var_lock, long *var, long new_value);
long	mutex_var_read(pthread_mutex_t *var_lock, long *var);
long	mutex_var_change_plus(pthread_mutex_t *var_lock, long *var);
void 	small_sleep(t_input_data *data, long time_ms);
long	time_in_mill(void);
void 	take_forks(t_philo *philo);
void 	drop_forks(t_philo *philo);
void	*gc_malloc(size_t size);
// void 	print_time(t_input_data *data, long time);

#endif