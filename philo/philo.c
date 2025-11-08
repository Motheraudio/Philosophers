#include "philo.h"

// int	create_philos(t_philo *sophers)
// {
//
// }

void	destroy_prev_forks(pthread_mutex_t **forks, ssize_t i)
{
	while(--i != -1)
		pthread_mutex_destroy(forks[i]);
	free(*forks);
	*forks = NULL;
}
int	create_forks(t_philo *sophers)
{
	ssize_t	i;

	i = -1;
	sophers->forks = malloc(sophers->philo_count + 1 * 
						 sizeof(pthread_mutex_t));
	if (sophers->forks == NULL)
		return (ft_putstr_fd("forks alloc failed", 2), 0);
	while(++i < sophers->philo_count)
	{
		if (pthread_mutex_init(&sophers->forks[i], NULL) == -1)
			return (ft_putstr_fd("mutex creation fail", 2),
				destroy_prev_forks(&sophers->forks, i), 0);
	}
	return (1);
}

int	get_time_atomic(t_philo *sophers)
{
	struct timeval	start_time;

	if (gettimeofday(&start_time, NULL) == -1)
		return(ft_putstr_fd("how did u break gettimeofday", 2), 0);
	atomic_init(&(sophers->atomic_mstime), start_time.tv_sec * 1000 + start_time.tv_usec / 1000);
	return (1);
}
// int	join_philos (t_philo *sophers)
// {
//
// }
#include <stdio.h>
int	main(int argc, char **argv)
{
	t_philo	sophers;

	if (!get_time_atomic(&sophers))
		return (1);
	printf("%zu", sophers.atomic_mstime);
	if (parse_and_store(&sophers, argc, argv) == -1)
		return (1);
	if (!create_forks(&sophers))
		return (1);
	
}
