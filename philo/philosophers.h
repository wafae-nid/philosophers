/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: wnid-hsa <wnid-hsa@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/17 02:40:05 by wnid-hsa          #+#    #+#             */
/*   Updated: 2025/08/24 03:52:58 by wnid-hsa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <unistd.h>
# include <limits.h>
# include <stdlib.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_input_data
{
	long			philo_num;
	long			t_die;
	long			t_eat;
	long			t_sleep;
	long			meals_nbr;
	pthread_mutex_t	print_lock;
	long			philos_born;
	long			end;
	long			current_time;
	long			all_full;
	pthread_mutex_t	v_lock;
}			t_input_data;

typedef struct s_fork
{
	pthread_mutex_t	fork;
	long			fork_nbr;
}	t_fork;

typedef struct s_philo
{
	pthread_t		philo_id;
	long			last_meal_time;
	t_fork			*left_fork;
	t_fork			*right_fork;
	long			meals_tracker;
	int				philo_position;
	long			full;
	pthread_mutex_t	philo_mtx;
	long			thread_fail;
	t_input_data	*data;
}	t_philo;

typedef struct s_helper
{
	long	curr_time;
	long	last_meal;
	long	death_time;
	int		meals_eaten;
}	t_helper;

typedef struct s_trash
{
	void			*point;
	struct s_trash	*next;
}					t_trash;

int				ft_is_a_numb(char c);
int				all_valid_chars(int argc, char **argv);
long			atol(const char *str);
int				all_checkings(int argc, char **argv);
void			print_input_error(int flag);
void			mutex_printf(t_input_data *data, int flag, int id);
void			mutex_var_change(pthread_mutex_t *v_lock,
					long *var, long new_value);
long			mutex_read(pthread_mutex_t *v_lock, long *var);
long			mutex_var_change_plus(pthread_mutex_t *v_lock, long *var);
void			small_sleep(t_input_data *data, long time_ms);
long			time_in_mill(void);
void			take_forks(t_philo *philo);
void			fork_take(t_philo **philos, t_fork **forks, int i);
void			drop_forks(t_philo *philo);
t_fork			**fork_arry_saving(t_fork **fork);
void			*gc_malloc(size_t size);
t_input_data	*fill_data_struct(char **argv);
t_fork			**fork_struct_alloc(t_input_data *data, t_fork **forks);
// t_philo			**philo_struct_alloc(t_input_data *data, t_fork **forks,
// 					t_philo **philos);
t_philo	**philo_struct_alloc(t_input_data *data, t_fork **forks,
	t_philo **philos, int i);
t_philo			**philo_birth(t_input_data	**data);
void			start_philo_execution(t_input_data	**data);
void			thread_cleaning(t_philo **philos, int i);
void			destroy_mutexes(t_philo **philos, t_input_data **data);
void			are_they_all_full(t_philo **philos);
void			*actions(void *arg);
void			*tracker_function(void *arg);

#endif
