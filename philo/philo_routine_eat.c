#include "philo.h"

void	philo_eat_one(t_philo *philo)
{
	pthread_mutex_lock(philo->left_fork);
	print_state(philo, "has taken a fork");
	while (!is_simulation_over(philo->table))
		usleep(100);
	pthread_mutex_unlock(philo->left_fork);
}

void	get_forks(t_philo *philo, pthread_mutex_t **first,
				pthread_mutex_t **second)
{
	if (philo->left_fork < philo->right_fork)
	{
		*first = philo->left_fork;
		*second = philo->right_fork;
	}
	else
	{
		*first = philo->right_fork;
		*second = philo->left_fork;
	}
}

void	philo_eat(t_philo *philo)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	if (philo->table->num_philos == 1)
		return (philo_eat_one(philo));
	get_forks(philo, &first, &second);
	pthread_mutex_lock(first);
	print_state(philo, "has taken a fork");
	pthread_mutex_lock(second);
	print_state(philo, "has taken a fork");
	if (is_simulation_over(philo->table))
	{
		pthread_mutex_unlock(second);
		pthread_mutex_unlock(first);
		return ;
	}
	print_state(philo, "is eating");
	pthread_mutex_lock(&philo->meal_mutex);
	philo->last_meal_time = get_time();
	philo->meals_eaten++;
	pthread_mutex_unlock(&philo->meal_mutex);
	ft_usleep(philo->table->time_to_eat, philo->table);
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
}
