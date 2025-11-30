/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routines.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvcampo <alvcampo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 18:58:34 by alvcampo          #+#    #+#             */
/*   Updated: 2025/11/30 16:50:16 by alvcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*think_routine(void *ids)
{
	t_id	*cast_id;

	cast_id = (t_id *) ids;
	if (atomic_load(cast_id->death) > 0)
		return (NULL);
	if (atomic_load(cast_id->ttd) <= get_time()
		- atomic_load(&cast_id->last_ate))
		return (atomic_fetch_add(cast_id->death, 1),
			print_mutex(cast_id, NULL), NULL);
	print_mutex(cast_id, "is thinking");
	if (!my_usleep(cast_id, atomic_load(cast_id->ttt)))
		return (NULL);
	if (atomic_load(cast_id->death) > 0)
		return (NULL);
	if (atomic_load(cast_id->ttd) <= get_time()
		- atomic_load(&cast_id->last_ate))
		return (atomic_fetch_add(cast_id->death, 1),
			print_mutex(cast_id, NULL), NULL);
	return (NULL);
}

void	*test_routine(void *ids)
{
	t_id	*cast_id;

	cast_id = (t_id *) ids;
	if (atomic_load(cast_id->eat_count) == NA)
		while (atomic_load(cast_id->death) == 0)
			usleep(10);
	else
		while (atomic_load(cast_id->death) == 0
			&& atomic_load(cast_id->end) == 0)
			usleep(10);
	return (NULL);
}

void	*sleep_routine(void *ids)
{
	t_id	*cast_id;

	cast_id = (t_id *) ids;
	if (atomic_load(cast_id->death) > 0)
		return (NULL);
	if (atomic_load(cast_id->ttd) <= get_time()
		- atomic_load(&cast_id->last_ate))
		return (atomic_fetch_add(cast_id->death, 1),
			print_mutex(cast_id, NULL), NULL);
	print_mutex(cast_id, "is sleeping");
	if (!my_usleep(cast_id, atomic_load(cast_id->tts)))
		return (NULL);
	return (NULL);
}

void	*one_philo_routine(void *ids)
{
	t_id	*cast_id;

	cast_id = (t_id *) ids;
	pthread_mutex_lock(cast_id->forks[RIGHT]);
	print_mutex(cast_id, "has taken a fork");
	usleep(atomic_load(cast_id->ttd));
	print_mutex(cast_id, NULL);
	pthread_mutex_unlock(cast_id->forks[RIGHT]);
	atomic_fetch_add(cast_id->death, 1);
	return (NULL);
}

void	*eat_routine(void *ids)
{
	t_id	*cast_id;

	cast_id = (t_id *) ids;
	if (atomic_load(cast_id->death) > 0)
		return (NULL);
	if (!pick_forks(cast_id, NULL))
		return (atomic_fetch_add(cast_id->death, 1),
			print_mutex(cast_id, NULL), NULL);
	if (atomic_load(cast_id->eat_count) != NA
		&& atomic_load(&cast_id->times_eaten)
		== atomic_load(cast_id->eat_count))
		return (atomic_fetch_add(cast_id->end, 1), NULL);
	return (NULL);
}
