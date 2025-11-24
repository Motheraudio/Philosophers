/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvcampo <alvcampo@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 18:52:23 by alvcampo          #+#    #+#             */
/*   Updated: 2025/11/24 19:42:15 by alvcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	create_philos(t_philo *sophers)
{
	sophers->philosophers = malloc((sophers->philo_count + 1)
			* sizeof(pthread_t));
	if (!sophers->philosophers)
		return (0);
	return (1);
}

void	join_prev_threads(t_philo *sophers, ssize_t i)
{
	ssize_t	j;

	j = 0;
	while (j < i)
	{
		pthread_join(sophers->philosophers[j], NULL);
		j++;
	}
	free(sophers->philosophers);
	sophers->philosophers = NULL;
}

int	init_threads(t_philo *sophers)
{
	ssize_t	i;

	i = -1;
	while (++i <= sophers->philo_count)
	{
		if (pthread_create(&sophers->philosophers[i], NULL, &start_routine,
				(void *)&sophers->ids[i]) != 0)
			return (join_prev_threads(sophers, i), 0);
	}
	return (1);
}
