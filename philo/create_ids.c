/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_ids.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvcampo <alvcampo@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 18:22:12 by alvcampo          #+#    #+#             */
/*   Updated: 2025/11/24 19:14:33 by alvcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	give_forks(t_philo *sophers, int i)
{
	sophers->ids[i].print_mutex = &sophers->forks[0];
	if (i == 0)
		return ;
	else if (i == 1)
	{
		sophers->ids[i].forks[0] = &sophers->forks[sophers->philo_count];
		sophers->ids[i].forks[1] = &sophers->forks[1];
	}
	else if (i == sophers->philo_count)
	{
		sophers->ids[i].forks[0] = &sophers->forks[sophers->philo_count];
		sophers->ids[i].forks[1] = &sophers->forks[1];
	}
	else
	{
		sophers->ids[i].forks[0] = &sophers->forks[i];
		sophers->ids[i].forks[1] = &sophers->forks[i + 1];
	}
}

void	print_mutex(t_id *ids, int message, atomic_size_t time)
{
	static atomic_int	printdeath = 0;
	atomic_size_t		time_ms;

	pthread_mutex_lock(ids->print_mutex);
	if (atomic_load(&printdeath) > 0)
		return ((void)pthread_mutex_unlock(ids->print_mutex));
	atomic_store(&time_ms,
		(atomic_load(&time) - atomic_load(ids->start_time)) / 1000);
	if (message == FORK)
		printf("%zu %i has taken a fork\n", time_ms, ids->number);
	else if (message == EAT)
		printf("%zu %i is eating\n", time_ms, ids->number);
	else if (message == SLEEP)
		printf("%zu %i is sleeping\n", time_ms, ids->number);
	else if (message == THINK)
		printf("%zu %i is thinking\n", time_ms, ids->number);
	else if (message == DIE)
	{
		atomic_fetch_add(&printdeath, 1);
		printf("%zu %i died\n", time_ms, ids->number);
		usleep(300);
	}
	pthread_mutex_unlock(ids->print_mutex);
}

static void	decide_start_routine(t_philo *sophers, ssize_t i)
{
	if (i == 0)
		sophers->ids[i].start_routine = test_routine;
	else if (sophers->philo_count == 1)
		sophers->ids[i].start_routine = one_philo_routine;
	else if (i % 2 == 0)
		sophers->ids[i].start_routine = eat_routine;
	else
		sophers->ids[i].start_routine = think_routine;
}

int	create_ids(t_philo *sophers)
{
	ssize_t	i;

	i = -1;
	sophers->ids = malloc((sophers->philo_count + 1) * sizeof(t_id));
	if (!sophers->ids)
		return (ft_putstr_fd("ID allocation failed", 2), 0);
	while (++i <= sophers->philo_count)
	{
		atomic_init(&sophers->ids[i].number, i);
		atomic_init(&sophers->ids[i].last_ate, 0);
		atomic_init(&sophers->ids[i].times_eaten, 0);
		atomic_init(&sophers->ids[i].atomic_p_count, sophers->philo_count);
		give_forks(sophers, i);
		sophers->ids[i].start = &sophers->start;
		sophers->ids[i].death = &sophers->death;
		sophers->ids[i].tte = &sophers->tte;
		sophers->ids[i].tts = &sophers->tts;
		sophers->ids[i].ttd = &sophers->ttd;
		sophers->ids[i].ttt = &sophers->ttt;
		sophers->ids[i].end = &sophers->end;
		sophers->ids[i].start_time = &sophers->atomic_ustime;
		sophers->ids[i].eat_count = &sophers->eat_count;
		if (atomic_load(&sophers->ids[i].number) % 2 == 0)
			sophers->ids[i].timetothink = sophers->tte;
		else
			sophers->ids[i].timetothink = sophers->tte * 2 - sophers->tts;
	//time to eat x 2 - tts <- impares, tte pares.]
		decide_start_routine(sophers, i);
	}
	return (1);
}
