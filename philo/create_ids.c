/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_ids.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvcampo <alvcampo@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 18:22:12 by alvcampo          #+#    #+#             */
/*   Updated: 2025/11/09 18:53:08 by alvcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void itoa_4(char dst[4], int src)
{
	if (src >= 0 && src <= 9)
	{
		dst[0] = src + 48;
		dst[1] = 0;
	}
	else if (src >= 10 && src <= 99)
	{
		dst[0] = src / 10 + 48;
		dst[1] = src % 10 + 48;
		dst[2] = 0;
	}
	else
	{
		dst[0] = src / 100 + 48;
		dst[1] = src / 10 + 48;
		dst[2] = src % 10 + 48;
		dst[3] = 0;
	}
}

void	give_forks(t_philo *sophers, int i)
{
	sophers->ids[i].print_mutex = &sophers->forks[0];
	if (i == 0)
		return ;
	sophers->ids[i].forks[0] = &sophers->forks[i];
	if (i == 1)
		sophers->ids[i].forks[1] = &sophers->forks[sophers->philo_count];
	else if (i == sophers->philo_count)
		sophers->ids[i].forks[1] = &sophers->forks[1];
	else
		sophers->ids[i].forks[1] = &sophers->forks[i + 1];

}

void	*test_routine(void* ids)
{
	t_id *cast_id;

	cast_id = (t_id *) ids;
	if (cast_id->atomic_p_count == 1)
	{
		while (*cast_id->death == 0)
			usleep(1);
		*cast_id->death += 1;
		return (NULL);
	}
	while (1) //*cast_id->death == 0 && *cast_id->end == 1)
	{
		if (*cast_id->death != 0)
			break ;
		if (*cast_id->end != 0)
			break ;
		// printf("%i, %i\n", atomic_load(cast_id->death), atomic_load(cast_id->end));
		// usleep(1);
	}
	*cast_id->death += 1;
	*cast_id->end += 1;
	return (NULL);
}

void	*print_routine(void* ids)
{
	t_id *cast_id;
	cast_id = (t_id *) ids;
	return (NULL);
}


void	print_mutex(t_id *ids, int message, atomic_size_t time)
{
	static atomic_int printdeath = 0;
	size_t	time_ms;

	(void) message;
	if (printdeath > 0)
		return ;
	if (pthread_mutex_lock(ids->print_mutex) != 0)
		return((void)ft_putstr_fd("print mutex couldn't be locked\n", 2));
	time_ms = (atomic_load(&time) - atomic_load(ids->start_time)) / 1000;
	if (message == FORK)
		printf("%zu %i has taken a fork\n", time_ms, ids->number); // add time later
	else if (message == EAT)
		printf("%zu %i is eating\n", time_ms, ids->number);
	else if (message == SLEEP)
		printf("%zu %i is sleeping\n", time_ms, ids->number);
	else if (message == THINK)
		printf("%zu %i is thinking\n", time_ms, ids->number);
	else if (message == DIE)
	{
	 	printdeath++;
		printf("%zu %i died\n", time_ms, ids->number);
	 	usleep(200);
	}
	pthread_mutex_unlock(ids->print_mutex);
}

void	*think_routine(void* ids)
{
	t_id			*cast_id;
	atomic_size_t	time;

	cast_id = (t_id *) ids;
	get_time_atomic(&time);
	if (*cast_id->death > 0)
		return(*cast_id->death += 1, NULL);
	if (*cast_id->ttd <= time - cast_id->last_ate)
		return (*cast_id->death += 1, print_mutex(cast_id, DIE, time), NULL);
	print_mutex(cast_id, THINK, time);
	usleep(*cast_id->ttt);
	get_time_atomic(&time);
	if (*cast_id->death > 0)
		return(*cast_id->death += 1, NULL);
	if (*cast_id->ttd <= time - cast_id->last_ate)
		return (*cast_id->death += 1, print_mutex(cast_id, DIE, time), NULL);
	return(eat_routine(cast_id));

}
void	*sleep_routine(void* ids)
{
	t_id			*cast_id;
	atomic_size_t	time;
	cast_id = (t_id *) ids;
	get_time_atomic(&time);
	if (*cast_id->death > 0)
		return(*cast_id->death += 1, NULL);
	if (*cast_id->ttd <= time - cast_id->last_ate)
		return (*cast_id->death += 1, print_mutex(cast_id, DIE, time), NULL);
	usleep(*cast_id->tts);
	get_time_atomic(&time);
	if (*cast_id->death > 0)
		return(*cast_id->death += 1, NULL);
	if (*cast_id->ttd <= time - cast_id->last_ate)
		return (*cast_id->death += 1, print_mutex(cast_id, DIE, time), NULL);
	print_mutex(cast_id, SLEEP, time);
	return(think_routine(cast_id));
}

void	pick_forks(t_id *cast_id, atomic_size_t *time)
{
	if (cast_id->number % 2 == 0)
	{
		pthread_mutex_lock(cast_id->forks[0]);
		get_time_atomic(time);
		print_mutex(cast_id, FORK, *time);
		get_time_atomic(time);
		pthread_mutex_lock(cast_id->forks[1]);
		print_mutex(cast_id, FORK, *time);
		get_time_atomic(time);
		print_mutex(cast_id, EAT, *time);
		cast_id->last_ate = *time;
		usleep(*cast_id->tte);
		pthread_mutex_unlock(cast_id->forks[0]);
		pthread_mutex_unlock(cast_id->forks[1]);
	}
	else
	{
		pthread_mutex_lock(cast_id->forks[1]);
		get_time_atomic(time);
		print_mutex(cast_id, FORK, *time);
		get_time_atomic(time);
		pthread_mutex_lock(cast_id->forks[0]);
		print_mutex(cast_id, FORK, *time);
		get_time_atomic(time);
		print_mutex(cast_id, EAT, *time);
		cast_id->last_ate = *time;
		usleep(*cast_id->tte);
		pthread_mutex_unlock(cast_id->forks[1]);
		pthread_mutex_unlock(cast_id->forks[0]);
	}
		cast_id->times_eaten++;
}
void	*eat_routine(void *ids)
{
	t_id			*cast_id;
	atomic_size_t	time;

	cast_id = (t_id *) ids;
	get_time_atomic(&time);
	if (*cast_id->death > 0)
		return(*cast_id->death += 1, NULL);
	if (*cast_id->ttd <= time - cast_id->last_ate)
		return (*cast_id->death += 1, print_mutex(cast_id, DIE, time), NULL);
	pick_forks(cast_id, &time);
	if (*cast_id->death > 0)
		return(*cast_id->death += 1, NULL);
	if (*cast_id->ttd <= time - cast_id->last_ate)
		return (*cast_id->death += 1, print_mutex(cast_id, DIE, time), NULL);
	if (*cast_id->eat_count != NA)
		if (cast_id->times_eaten == *cast_id->eat_count)
			return (*cast_id->end += 1, NULL);
	return(sleep_routine(cast_id));
}

void	*god_routine(void* ids)
{
	(void)ids;

	return (NULL);
}

void *one_philo_routine(void *ids)
{
	t_id			*cast_id;
	atomic_size_t	time;

	cast_id = (t_id *) ids;
	get_time_atomic(&time);
	pthread_mutex_lock(cast_id->forks[0]);
	print_mutex(cast_id, FORK, time);
	usleep(*cast_id->ttd);
	get_time_atomic(&time);
	print_mutex(cast_id, DIE, time);
	*cast_id->death += 1;
	return (NULL);
}
int	create_ids(t_philo *sophers)
{
	int	i;

	i = -1;
	sophers->ids = malloc((sophers->philo_count + 1) * sizeof(t_id));
	if (!sophers->ids)
		return (ft_putstr_fd("ID allocation failed", 2), 0);
	while (++i <= sophers->philo_count)
	{
		atomic_init(&sophers->ids[i].number, i);
		itoa_4(sophers->ids[i].name, i);
		atomic_init(&sophers->ids[i].last_ate, 0);
		atomic_init(&sophers->ids[i].times_eaten, 0);
		atomic_init(&sophers->ids[i].atomic_p_count, sophers->philo_count);
		give_forks(sophers, i);
		sophers->ids[i].buffer = sophers->buffer;
		sophers->ids[i].start = &sophers->start;
		sophers->ids[i].death = &sophers->death;
		sophers->ids[i].state = &sophers->states[i];
		sophers->ids[i].tte = &sophers->tte;
		sophers->ids[i].tts = &sophers->tts;
		sophers->ids[i].ttd = &sophers->ttd;
		sophers->ids[i].ttt = &sophers->ttt;
		sophers->ids[i].end = &sophers->end;
		sophers->ids[i].start_time = &sophers->atomic_ustime;
		sophers->ids[i].eat_count = &sophers->eat_count;
		if (i == 0)
			sophers->ids[i].start_routine = test_routine;
		else if (sophers->philo_count == 1)
			sophers->ids[i].start_routine = one_philo_routine;
		else if(i % 2 == 0)
			sophers->ids[i].start_routine = eat_routine;
		else
			sophers->ids[i].start_routine = think_routine;

		// call func that adds starting routine
	}
	return (1);
}
