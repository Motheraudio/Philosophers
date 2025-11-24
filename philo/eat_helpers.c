/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvcampo <alvcampo@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 19:04:43 by alvcampo          #+#    #+#             */
/*   Updated: 2025/11/24 19:15:12 by alvcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	select_fork(t_id *cast_id, pthread_mutex_t **f, pthread_mutex_t **s)
{
	if (cast_id->forks[0] < cast_id->forks[1])
	{
		*f = cast_id->forks[0];
		*s = cast_id->forks[1];
	}
	else
	{
		*f = cast_id->forks[1];
		*s = cast_id->forks[0];
	}
}

int	pick_forks(t_id *cast_id, atomic_size_t *time)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	select_fork(cast_id, &first, &second);
	pthread_mutex_lock(first);
	get_time_atomic(time);
	if (*cast_id->ttd <= *time - cast_id->last_ate)
		return (pthread_mutex_unlock(second), pthread_mutex_unlock(first), 0);
	print_mutex(cast_id, FORK, *time);
	pthread_mutex_lock(second);
	get_time_atomic(time);
	if (*cast_id->ttd <= *time - cast_id->last_ate)
		return (pthread_mutex_unlock(second), pthread_mutex_unlock(first), 0);
	print_mutex(cast_id, FORK, *time);
	print_mutex(cast_id, EAT, *time);
	get_time_atomic(time);
	if (*cast_id->ttd <= *time - cast_id->last_ate)
		return (pthread_mutex_unlock(second), pthread_mutex_unlock(first), 0);
	usleep(*cast_id->tte);
	get_time_atomic(time);
	cast_id->last_ate = *time;
	pthread_mutex_unlock(second);
	pthread_mutex_unlock(first);
	cast_id->times_eaten++;
	return (1);
}
