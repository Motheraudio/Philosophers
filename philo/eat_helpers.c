/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_helpers.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvcampo <alvcampo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 19:04:43 by alvcampo          #+#    #+#             */
/*   Updated: 2025/11/30 00:16:34 by alvcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	select_fork(t_id *cast_id, pthread_mutex_t **f, pthread_mutex_t **s)
{
	if (cast_id->number == atomic_load(&cast_id->atomic_p_count))
	{
		*f = cast_id->forks[1];
		*s = cast_id->forks[0];
	}
	else
	{
		*f = cast_id->forks[0];
		*s = cast_id->forks[1];
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

	(void) time;
	select_fork(cast_id, &first, &second);
	pthread_mutex_lock(first);
	if (atomic_load(cast_id->ttd) <= get_time() 
		- atomic_load(&cast_id->last_ate))
		return (pthread_mutex_unlock(first), 0);
	print_mutex(cast_id, "has taken a fork");
	pthread_mutex_lock(second);
	if (atomic_load(cast_id->ttd) <= get_time() - atomic_load(&cast_id->last_ate))
		return (pthread_mutex_unlock(second), pthread_mutex_unlock(first), 0);
	cast_id->last_ate = get_time();
	print_mutex(cast_id, "has taken a fork");
	print_mutex(cast_id, "is eating");
	// get_time_atomic(time);
	// if (atomic_load(cast_id->ttd) <= atomic_load(time)
	// 	- atomic_load(&cast_id->last_ate))
	// 	return (pthread_mutex_unlock(second), pthread_mutex_unlock(first), 0);
	if (!my_usleep(cast_id, atomic_load(cast_id->tte)))
		return (pthread_mutex_unlock(second), pthread_mutex_unlock(first), 0);
	return (finish_ate(cast_id, first, second), 1);
}
