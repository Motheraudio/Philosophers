/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_forks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvcampo <alvcampo@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 09:51:41 by alvcampo          #+#    #+#             */
/*   Updated: 2025/11/09 19:09:13 by alvcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdio.h>

void	destroy_prev_forks(t_philo *sophers, ssize_t i)
{
	ssize_t	j;
	
	j = -1;
	while (++j <= i)
		pthread_mutex_destroy(&sophers->forks[i]);
	free(sophers->forks);
	sophers->forks = NULL;
}

int	create_forks(t_philo *sophers)
{
	ssize_t	i;

	i = -1;
	sophers->forks = malloc((sophers->philo_count + 1)
			* sizeof(pthread_mutex_t));
	if (sophers->forks == NULL)
		return (ft_putstr_fd("forks alloc failed", 2), 0);
	while (++i <= sophers->philo_count)
	{
		if (pthread_mutex_init(&sophers->forks[i], NULL) != 0)
			return (ft_putstr_fd("mutex creation fail", 2),
				destroy_prev_forks(sophers, i), 0);
	}
	return (1);
}
