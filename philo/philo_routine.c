/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_routine.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tmorais- <tmorais-@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/28 15:09:08 by tmorais-          #+#    #+#             */
/*   Updated: 2026/05/28 15:39:04 by tmorais-         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	philo_think(t_philo *philo)
{
	long long	think_time;

	print_state(philo, "is thinking");
	if (philo->table->num_philos % 2 == 0)
		return ;
	think_time = 2 * philo->table->time_to_eat - philo->table->time_to_sleep;
	if (think_time < 0)
		think_time = 0;
	ft_usleep(think_time, philo->table);
}

void	philo_sleep_think(t_philo *philo)
{
	print_state(philo, "is sleeping");
	ft_usleep(philo->table->time_to_sleep, philo->table);
	if (is_simulation_over(philo->table))
		return ;
	philo_think(philo);
}

int	is_full(t_philo *philo)
{
	int	full;

	if (philo->table->must_eat_count == -1)
		return (0);
	pthread_mutex_lock(&philo->meal_mutex);
	full = (philo->meals_eaten >= philo->table->must_eat_count);
	pthread_mutex_unlock(&philo->meal_mutex);
	return (full);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2 == 0)
		usleep(1000);
	while (!is_simulation_over(philo->table))
	{
		if (is_full(philo))
			break ;
		philo_eat(philo);
		if (is_simulation_over(philo->table))
			break ;
		philo_sleep_think(philo);
	}
	return (NULL);
}
