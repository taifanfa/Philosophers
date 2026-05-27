#include "philo.h"

static int	start_simulation(t_table *table)
{
	int			i;
	pthread_t	monitor;

	i = 0;
	while (i < table->num_philos)
	{
		if (pthread_create(&table->philos[i].thread, NULL,
				philo_routine, &table->philos[i]) != 0)
			return (1);
		i++;
	}
	if (pthread_create(&monitor, NULL, monitor_routine, table) != 0)
		return (1);
	i = 0;
	while (i < table->num_philos)
	{
		pthread_join(table->philos[i].thread, NULL);
		i++;
	}
	pthread_join(monitor, NULL);
	return (0);
}

static int	check_args(int argc, char **argv)
{
	int	i;
	int	j;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: ./philo num_philos time_to_die time_to_eat ");
		printf("time_to_sleep [must_eat_count]\n");
		return (1);
	}
	i = 1;
	while (i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_table	table;

	if (check_args(argc, argv) != 0)
	{
		printf("Error: invalid arguments\n");
		return (1);
	}
	memset(&table, 0, sizeof(t_table));
	if (init_table(&table, argc, argv) != 0)
	{
		printf("Error: initialization failed\n");
		return (1);
	}
	if (start_simulation(&table) != 0)
	{
		printf("Error: simulation failed\n");
		cleanup(&table);
		return (1);
	}
	cleanup(&table);
	return (0);
}
