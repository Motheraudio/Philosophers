/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvcampo <alvcampo@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 19:04:43 by alvcampo          #+#    #+#             */
/*   Updated: 2025/11/24 20:12:23 by alvcampo         ###   ########.fr       */
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

static void	finish_ate(t_id *cast_id, pthread_mutex_t *f, pthread_mutex_t *s)
{
	pthread_mutex_unlock(s);
	pthread_mutex_unlock(f);
	atomic_fetch_add(&cast_id->times_eaten, 1);
}

int	pick_forks(t_id *cast_id, atomic_size_t *time)
{
	pthread_mutex_t	*first;
	pthread_mutex_t	*second;

	select_fork(cast_id, &first, &second);
	pthread_mutex_lock(first);
	get_time_atomic(time);
	if (atomic_load(cast_id->ttd) <= atomic_load(time)
		- atomic_load(&cast_id->last_ate))
		return (pthread_mutex_unlock(second), pthread_mutex_unlock(first), 0);
	print_mutex(cast_id, FORK, atomic_load(time));
	pthread_mutex_lock(second);
	get_time_atomic(time);
	if (atomic_load(cast_id->ttd) <= atomic_load(time)
		- atomic_load(&cast_id->last_ate))
		return (pthread_mutex_unlock(second), pthread_mutex_unlock(first), 0);
	print_mutex(cast_id, FORK, atomic_load(time));
	print_mutex(cast_id, EAT, atomic_load(time));
	get_time_atomic(time);
	if (atomic_load(cast_id->ttd) <= atomic_load(time)
		- atomic_load(&cast_id->last_ate))
		return (pthread_mutex_unlock(second), pthread_mutex_unlock(first), 0);
	usleep(atomic_load(cast_id->tte));
	get_time_atomic(time);
	atomic_store(&cast_id->last_ate, atomic_load(time));
	return (finish_ate(cast_id, first, second), 1);
}
