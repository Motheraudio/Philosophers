/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvcampo <alvcampo@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 18:58:34 by alvcampo          #+#    #+#             */
/*   Updated: 2025/11/24 20:02:53 by alvcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*think_routine(void *ids)
{
	t_id			*cast_id;
	atomic_size_t	time;

	cast_id = (t_id *) ids;
	get_time_atomic(&time);
	if (atomic_load(cast_id->death) > 0)
		return (NULL);
	if (atomic_load(cast_id->ttd) <= atomic_load(&time)
		- atomic_load(&cast_id->last_ate))
		return (atomic_fetch_add(cast_id->death, 1),
			print_mutex(cast_id, DIE, time), NULL);
	print_mutex(cast_id, THINK, time);
	usleep(atomic_load(cast_id->ttt));
	get_time_atomic(&time);
	if (atomic_load(cast_id->death) > 0)
		return (NULL);
	if (atomic_load(cast_id->ttd) <= atomic_load(&time)
		- atomic_load(&cast_id->last_ate))
		return (atomic_fetch_add(cast_id->death, 1),
			print_mutex(cast_id, DIE, time), NULL);
	return (eat_routine(cast_id));
}

void	*test_routine(void *ids)
{
	t_id	*cast_id;

	cast_id = (t_id *) ids;
	if (atomic_load(cast_id->eat_count) == NA)
		while (atomic_load(cast_id->death) == 0)
			usleep(1);
	else
		while (atomic_load(cast_id->death) == 0
			&& atomic_load(cast_id->end) == 0)
			usleep(1);
	return (NULL);
}

void	*sleep_routine(void *ids)
{
	t_id			*cast_id;
	atomic_size_t	time;

	cast_id = (t_id *) ids;
	get_time_atomic(&time);
	print_mutex(cast_id, SLEEP, time);
	if (atomic_load(cast_id->death) > 0)
		return (NULL);
	if (atomic_load(cast_id->ttd) <= atomic_load(&time)
		- atomic_load(&cast_id->last_ate))
		return (atomic_fetch_add(cast_id->death, 1),
			print_mutex(cast_id, DIE, time), NULL);
	usleep(atomic_load(cast_id->tts));
	get_time_atomic(&time);
	if (atomic_load(cast_id->death) > 0)
		return (NULL);
	if (atomic_load(cast_id->ttd) <= atomic_load(&time)
		- atomic_load(&cast_id->last_ate))
		return (atomic_fetch_add(cast_id->death, 1),
			print_mutex(cast_id, DIE, time), NULL);
	return (think_routine(cast_id));
}

void	*one_philo_routine(void *ids)
{
	t_id			*cast_id;
	atomic_size_t	time;

	cast_id = (t_id *) ids;
	get_time_atomic(&time);
	pthread_mutex_lock(cast_id->forks[0]);
	print_mutex(cast_id, FORK, time);
	usleep(atomic_load(cast_id->ttd));
	get_time_atomic(&time);
	print_mutex(cast_id, DIE, time);
	atomic_fetch_add(cast_id->death, 1);
	return (NULL);
}

void	*eat_routine(void *ids)
{
	t_id			*cast_id;
	atomic_size_t	time;

	cast_id = (t_id *) ids;
	get_time_atomic(&time);
	if (atomic_load(cast_id->death) > 0)
		return (NULL);
	if (!pick_forks(cast_id, &time))
		return (atomic_fetch_add(cast_id->death, 1),
			print_mutex(cast_id, DIE, time), NULL);
	if (atomic_load(cast_id->death) > 0)
		return (NULL);
	if (atomic_load(cast_id->ttd) <= atomic_load(&time)
		- atomic_load(&cast_id->last_ate))
		return (atomic_fetch_add(cast_id->death, 1),
			print_mutex(cast_id, DIE, time), NULL);
	if (atomic_load(cast_id->eat_count) != NA
		&& atomic_load(&cast_id->times_eaten)
		== atomic_load(cast_id->eat_count))
		return (atomic_fetch_add(cast_id->end, 1), NULL);
	return (sleep_routine(cast_id));
}
