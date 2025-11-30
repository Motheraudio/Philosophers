/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_ids.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvcampo <alvcampo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 18:22:12 by alvcampo          #+#    #+#             */
/*   Updated: 2025/11/30 00:08:57 by alvcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	give_forks(t_philo *sophers, int i)
{
	sophers->ids[i].print_mutex = &sophers->forks[0];
	if (i == 0)
		return ;
	sophers->ids[i].forks[RIGHT] = &sophers->forks[i];
	if (i != sophers->philo_count)
		sophers->ids[i].forks[LEFT] = &sophers->forks[i + 1];
	else
		sophers->ids[i].forks[LEFT] = &sophers->forks[1];
}

void	print_mutex(t_id *ids, char *message)
{
	static atomic_int	printdeath = 0;

	if (atomic_load(&printdeath) > 0)
		return ;
	pthread_mutex_lock(ids->print_mutex);
	if (!message && atomic_load(&printdeath) == 0)
	{
		atomic_fetch_add(&printdeath, 1);
		printf("%zu %i died\n", (get_time()
				- ids->local_time) / 1000, ids->number);
		usleep(300);
	}
	else if (message && atomic_load(&printdeath) == 0)
	{
		printf("%zu %i %s\n", (get_time() - ids->local_time) / 1000,
			ids->number, message);
	}
	pthread_mutex_unlock(ids->print_mutex);
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
		sophers->ids[i].last_ate = 0;
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
	}
	return (1);
}
