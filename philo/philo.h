/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorais- <tmorais-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 15:09:14 by tmorais-          #+#    #+#             */
/*   Updated: 2026/05/28 15:29:32 by tmorais-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>
# include <string.h>

typedef struct s_table	t_table;

typedef struct s_philo
{
	int				id;
	int				meals_eaten;
	long long		last_meal_time;
	pthread_t		thread;
	pthread_mutex_t	*left_fork;
	pthread_mutex_t	*right_fork;
	pthread_mutex_t	meal_mutex;
	t_table			*table;
}	t_philo;

typedef struct s_table
{
	int				num_philos;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				must_eat_count;
	int				simulation_over;
	long long		start_time;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	death_mutex;
	t_philo			*philos;
}	t_table;

int			init_table(t_table *table, int argc, char **argv);
void		cleanup(t_table *table);
int			is_full(t_philo *philo);
void		destroy_meal_mutexes(t_table *table, int i);
void		get_forks(t_philo *philo, pthread_mutex_t **first,
				pthread_mutex_t **second);
void		philo_eat_one(t_philo *philo);
void		philo_eat(t_philo *philo);
void		philo_think(t_philo *philo);
void		philo_sleep_think(t_philo *philo);
void		*philo_routine(void *arg);
void		*monitor_routine(void *arg);
long long	get_time(void);
void		ft_usleep(long long ms, t_table *table);
void		print_state(t_philo *philo, char *state);
int			is_simulation_over(t_table *table);

#endif
