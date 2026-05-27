#include "philo.h"

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000LL));
}

void	ft_usleep(long long ms, t_table *table)
{
	long long	start;

	start = get_time();
	while ((get_time() - start) < ms)
	{
		if (table && is_simulation_over(table))
			return ;
		usleep(100);
	}
}

void	print_state(t_philo *philo, char *state)
{
	long long	timestamp;

	if (is_simulation_over(philo->table))
		return ;
	pthread_mutex_lock(&philo->table->print_mutex);
	if (!is_simulation_over(philo->table))
	{
		timestamp = get_time() - philo->table->start_time;
		printf("%lld %d %s\n", timestamp, philo->id, state);
	}
	pthread_mutex_unlock(&philo->table->print_mutex);
}

int	is_simulation_over(t_table *table)
{
	int	over;

	pthread_mutex_lock(&table->death_mutex);
	over = table->simulation_over;
	pthread_mutex_unlock(&table->death_mutex);
	return (over);
}
