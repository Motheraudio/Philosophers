/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvcampo <alvcampo@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 09:57:36 by alvcampo          #+#    #+#             */
/*   Updated: 2025/11/09 18:24:13 by alvcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
// int	create_philos(t_philo *sophers)
// {
//
// }
#include <stdio.h>
int	get_time_atomic(t_philo *sophers)
{
	struct timeval	start_time;

	if (gettimeofday(&start_time, NULL) == -1)
		return (ft_putstr_fd("how did u break gettimeofday", 2), 0);
	atomic_init(&(sophers->atomic_ustime),
		start_time.tv_sec * 1000000 + start_time.tv_usec); // this is in microseconds, for printing ineed miliseconds
	return (1);
}

int	create_states(t_philo *sophers)
{
	int	i;

	i = -1;
	sophers->states = malloc((sophers->philo_count + 1) * sizeof(atomic_int));
	if (sophers->states == NULL)
		return (ft_putstr_fd("State creation failed", 2), 0);
	while (++i < sophers->philo_count + 1)
		atomic_init(&sophers->states[i], 0);
	atomic_init(&sophers->start, 0);
	atomic_init(&sophers->death, 0);
	return (1);
}

int	main(int argc, char **argv)
{
	t_philo	sophers;

	memset(sophers.buffer, 0, 2000);
	if (!get_time_atomic(&sophers))
		return (1);
	if (parse_and_store(&sophers, argc, argv) == -1)
		return (1);
	if (!create_forks(&sophers))
		return (1);
	if (!create_states(&sophers))
		return (destroy_prev_forks(&sophers, sophers.philo_count), 1);
	if (!create_ids(&sophers))
		return (destroy_prev_forks(&sophers, sophers.philo_count),
			free(sophers.states), 1);
}
