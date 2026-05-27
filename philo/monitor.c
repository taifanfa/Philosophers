#include "philo.h"

static void	set_simulation_over(t_table *table)
{
	pthread_mutex_lock(&table->death_mutex);
	table->simulation_over = 1;
	pthread_mutex_unlock(&table->death_mutex);
}

static int	philo_is_dead(t_philo *philo, long long *death_time)
{
	long long	elapsed;
	long long	last_meal;
	long long	now;

	pthread_mutex_lock(&philo->meal_mutex);
	last_meal = philo->last_meal_time;
	pthread_mutex_unlock(&philo->meal_mutex);
	now = get_time();
	elapsed = now - last_meal;
	if (elapsed > philo->table->time_to_die)
	{
		*death_time = now - philo->table->start_time;
		return (1);
	}
	return (0);
}

static int	all_philos_full(t_table *table)
{
	int	i;
	int	meals;

	if (table->must_eat_count == -1)
		return (0);
	i = 0;
	while (i < table->num_philos)
	{
		pthread_mutex_lock(&table->philos[i].meal_mutex);
		meals = table->philos[i].meals_eaten;
		pthread_mutex_unlock(&table->philos[i].meal_mutex);
		if (meals < table->must_eat_count)
			return (0);
		i++;
	}
	return (1);
}

static int	check_deaths(t_table *table)
{
	int			i;
	long long	death_time;

	i = 0;
	while (i < table->num_philos)
	{
		death_time = 0;
		if (philo_is_dead(&table->philos[i], &death_time))
		{
			set_simulation_over(table);
			pthread_mutex_lock(&table->print_mutex);
			printf("%lld %d died\n", death_time, table->philos[i].id);
			pthread_mutex_unlock(&table->print_mutex);
			return (1);
		}
		i++;
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_table	*table;

	table = (t_table *)arg;
	while (!is_simulation_over(table))
	{
		if (check_deaths(table))
			break ;
		if (all_philos_full(table))
		{
			set_simulation_over(table);
			break ;
		}
		usleep(500);
	}
	return (NULL);
}
