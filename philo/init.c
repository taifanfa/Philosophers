#include "philo.h"

static int	init_forks(t_table *table)
{
	int	i;

	table->forks = malloc(sizeof(pthread_mutex_t) * table->num_philos);
	if (!table->forks)
		return (1);
	i = 0;
	while (i < table->num_philos)
	{
		if (pthread_mutex_init(&table->forks[i], NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

static int	init_philos(t_table *table)
{
	int	i;

	table->philos = malloc(sizeof(t_philo) * table->num_philos);
	if (!table->philos)
		return (1);
	i = 0;
	while (i < table->num_philos)
	{
		table->philos[i].id = i + 1;
		table->philos[i].meals_eaten = 0;
		table->philos[i].last_meal_time = table->start_time;
		table->philos[i].table = table;
		table->philos[i].left_fork = &table->forks[i];
		table->philos[i].right_fork = &table->forks[(i + 1) % table->num_philos];
		if (pthread_mutex_init(&table->philos[i].meal_mutex, NULL) != 0)
			return (1);
		i++;
	}
	return (0);
}

static void	parse_args(t_table *table, int argc, char **argv)
{
	table->num_philos = atoi(argv[1]);
	table->time_to_die = (long long)atoi(argv[2]);
	table->time_to_eat = (long long)atoi(argv[3]);
	table->time_to_sleep = (long long)atoi(argv[4]);
	table->must_eat_count = -1;
	if (argc == 6)
		table->must_eat_count = atoi(argv[5]);
	table->simulation_over = 0;
}

int	init_table(t_table *table, int argc, char **argv)
{
	parse_args(table, argc, argv);
	if (table->num_philos <= 0 || table->num_philos > 200)
		return (1);
	if (pthread_mutex_init(&table->print_mutex, NULL) != 0)
		return (1);
	if (pthread_mutex_init(&table->death_mutex, NULL) != 0)
		return (1);
	table->start_time = get_time();
	if (init_forks(table) != 0)
		return (1);
	if (init_philos(table) != 0)
		return (1);
	return (0);
}

void	cleanup(t_table *table)
{
	int	i;

	i = 0;
	while (i < table->num_philos)
	{
		pthread_mutex_destroy(&table->forks[i]);
		pthread_mutex_destroy(&table->philos[i].meal_mutex);
		i++;
	}
	pthread_mutex_destroy(&table->print_mutex);
	pthread_mutex_destroy(&table->death_mutex);
	free(table->forks);
	free(table->philos);
}
