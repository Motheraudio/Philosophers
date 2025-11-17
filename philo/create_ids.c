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
	return (NULL);
}

void	*print_routine(void* ids)
{
	t_id *cast_id;
	cast_id = (t_id *) ids;
	return (NULL);
}


void	print_mutex(t_id *ids, int message)
{
	if (pthread_mutex_lock(ids->print_mutex) != 0)
		return((void)ft_putstr_fd("print mutex couldn't be locked", 2));
	if (message == FORK)
		printf("%s has taken a fork", ids->name); // add time later
	else if (message == EAT)
		printf("%s is eating", ids->name);
	else if (message == SLEEP)
		printf("%s is sleeping", ids->name);
	else if (message == THINK)
		printf("%s is thinking", ids->name);
	else if (message == DIE)
	{
		printf("%s died", ids->name);
		pthread_mutex_unlock(ids->print_mutex);
		*ids->death += 1;
		return ;
	}
	pthread_mutex_unlock(ids->print_mutex);
}

void	*think_routine(void* ids)
{
	t_id			*cast_id;
	atomic_size_t	time;

	cast_id = (t_id *) ids;
	get_time_atomic(&time);
	if (*cast_id->death > 0 || *cast_id->ttd >= time - cast_id->last_ate)
		return (print_mutex(cast_id, DIE), NULL);
	usleep(*cast_id->ttt);
	print_mutex(cast_id, THINK);
	return(eat_routine(ids));

}
void	*sleep_routine(void* ids)
{
	t_id			*cast_id;
	atomic_size_t	time;

	cast_id = (t_id *) ids;
	get_time_atomic(&time);
	if (*cast_id->death > 0 || *cast_id->ttd >= time - cast_id->last_ate)
		return (print_mutex(cast_id, DIE), NULL);
	usleep(*cast_id->tts);
	print_mutex(cast_id, SLEEP);
	return(think_routine(ids));
}
void	*eat_routine(void *ids)
{
	t_id			*cast_id;
	atomic_size_t	time;

	cast_id = (t_id *) ids;
	get_time_atomic(&time);
	if (*cast_id->death > 0 || *cast_id->ttd >= time - cast_id->last_ate)
		return (print_mutex(cast_id, DIE), NULL);
	atomic_store(&cast_id->last_ate, time);
	pthread_mutex_lock(cast_id->forks[0]);
	print_mutex(cast_id, FORK);
	pthread_mutex_lock(cast_id->forks[1]);
	print_mutex(cast_id, FORK);
	print_mutex(cast_id, EAT);
	usleep(*cast_id->tte);
	return(sleep_routine(ids));
}

void	*god_routine(void* ids)
{
	t_id *cast_id;
	cast_id = (t_id *) ids;
	return (NULL);
}
int	create_ids(t_philo *sophers)
{
	int	i;

	i = -1;
	sophers->ids = malloc((sophers->philo_count + 1) * sizeof(t_id));
	if (!sophers->ids)
		return (ft_putstr_fd("ID allocation failed", 2), 0);
	while (++i < sophers->philo_count + 1)
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
		sophers->ids[i].start_time = &sophers->atomic_ustime;
		sophers->ids[i].eat_count = &sophers->eat_count;
		if (i == 0)
			sophers->ids[i].start_routine = NULL;
		if(i % 2 == 0)
			sophers->ids[i].start_routine = eat_routine;
		else
			sophers->ids[i].start_routine = think_routine;

		// call func that adds starting routine
	}
	return (1);
}
