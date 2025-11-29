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

void	routine_loop_odds(void *ids)
{
	t_id		*cast_id;
	
	cast_id = (t_id *)ids;
	while(1)
	{
		if (!sleep_routine(cast_id) && atomic_load(cast_id->death) != 0)
			return ;
		if (!think_routine(cast_id) && atomic_load(cast_id->death) != 0)
			return ;
		if (!eat_routine(cast_id) && atomic_load(cast_id->death) != 0)
			return ;
	}
}

void	routine_loop_even(void *ids)
{
	t_id		*cast_id;
	
	cast_id = (t_id *)ids;
	while(1)
	{
		if (!eat_routine(cast_id) && atomic_load(cast_id->death) != 0)
			return ;
		if (!sleep_routine(cast_id) && atomic_load(cast_id->death) != 0)
			return ;
		if (!think_routine(cast_id) && atomic_load(cast_id->death) != 0)
			return ;
	}
}
void	*think_routine(void *ids)
{
	t_id			*cast_id;
	atomic_size_t			time;

	cast_id = (t_id *) ids;
	get_time_atomic(&time);
	if (atomic_load(cast_id->death) > 0)
		return (NULL);
	if (atomic_load(cast_id->ttd) <= atomic_load(&time) - atomic_load(&cast_id->last_ate))
		return (atomic_fetch_add(cast_id->death, 1),
			print_mutex(cast_id, NULL, atomic_load(&time)), NULL);
	print_mutex(cast_id, "is thinking", atomic_load(&time));
	if (!my_usleep(cast_id, atomic_load(cast_id->ttt)))
		return (NULL);
	get_time_atomic(&time);
	if (atomic_load(cast_id->death) > 0)
		return (NULL);
	if (atomic_load(cast_id->ttd) <= atomic_load(&time) 
		- atomic_load(&cast_id->last_ate))
		return (atomic_fetch_add(cast_id->death, 1),
			print_mutex(cast_id, NULL, atomic_load(&time)), NULL);
	return (NULL);
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
	atomic_size_t			time;

	cast_id = (t_id *) ids;
	get_time_atomic(&time);
	if (atomic_load(cast_id->death) > 0)
		return (NULL);
	if (atomic_load(cast_id->ttd) <= time - atomic_load(&cast_id->last_ate))
		return (atomic_fetch_add(cast_id->death, 1),
			print_mutex(cast_id, NULL, time), NULL);
	print_mutex(cast_id, "is sleeping", atomic_load(&time));
		if (!my_usleep(cast_id, atomic_load(cast_id->tts)))
		return (NULL);
	return (NULL);
}

void	*one_philo_routine(void *ids)
{
	t_id			*cast_id;
	atomic_size_t	time;

	cast_id = (t_id *) ids;
	get_time_atomic(&time);
	pthread_mutex_lock(cast_id->forks[RIGHT]);
	print_mutex(cast_id, "has taken a fork", atomic_load(&time));
	usleep(atomic_load(cast_id->ttd));
	get_time_atomic(&time);
	print_mutex(cast_id, NULL, atomic_load(&time));
	pthread_mutex_unlock(cast_id->forks[RIGHT]);
	atomic_fetch_add(cast_id->death, 1);
	return (NULL);
}

void	*eat_routine(void *ids)
{
	t_id			*cast_id;
	atomic_size_t			time;

	cast_id = (t_id *) ids;
	get_time_atomic(&time);
	if (atomic_load(cast_id->death) > 0)
		return (NULL);
	if (!pick_forks(cast_id, &time))
		return (atomic_fetch_add(cast_id->death, 1),
			print_mutex(cast_id, NULL, atomic_load(&time)), NULL);
	if (atomic_load(cast_id->eat_count) != NA
		&& atomic_load(&cast_id->times_eaten)
		== atomic_load(cast_id->eat_count))
		return (atomic_fetch_add(cast_id->end, 1), NULL);
	return (NULL);
}
