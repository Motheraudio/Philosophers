/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvcampo <alvcampo@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 09:57:36 by alvcampo          #+#    #+#             */
/*   Updated: 2025/11/09 18:24:13 by alvcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
// int	create_philos(t_philo *sophers)
// {
//
// }
#include <stdio.h>

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*tmp_dst;
	unsigned char	*tmp_src;

	tmp_dst = (unsigned char *)dst;
	tmp_src = (unsigned char *)src;
	while (n > 0)
	{
		*tmp_dst = *tmp_src;
		tmp_dst++;
		tmp_src++;
		n--;
	}
	return (dst);
}

int	get_time_atomic(atomic_size_t *ustime)
{
	struct timeval	start_time;

	if (gettimeofday(&start_time, NULL) == -1)
		return (ft_putstr_fd("how did u break gettimeofday", 2), 0);
	atomic_init(ustime,
		start_time.tv_sec * 1000000 + start_time.tv_usec); // this is in microseconds, for printing ineed miliseconds
	return (1);
}

int	create_states(t_philo *sophers)
{
	int	i;

	i = -1;
	sophers->states = malloc((sophers->philo_count + 1) * sizeof(atomic_int));
	if (sophers->states == NULL)
		return (ft_putstr_fd("State creation failed\n", 2), 0);
	while (++i < sophers->philo_count + 1)
		atomic_init(&sophers->states[i], 0);
	atomic_init(&sophers->start, 0);
	atomic_init(&sophers->death, 0);
	return (1);
}

void join_prev_threads(t_philo *sophers, ssize_t i)
{
	ssize_t	j;
	
	j = -1;
	while (++j < i)
		pthread_join(sophers->philosophers[j], NULL);
	free(sophers->philosophers);
	sophers->philosophers = NULL;
}

void	*start_routine(void *ids)
{
	t_id	stack_ids;
	atomic_size_t	time;
	ft_memcpy((void *)&stack_ids, ids, sizeof(t_id));
	get_time_atomic(&time);
	stack_ids.last_ate = time;
	*stack_ids.start += 1;
	while(*stack_ids.start != stack_ids.atomic_p_count + 1)
		usleep(1);
	return (stack_ids.start_routine((void *)&stack_ids));
}
int	init_threads(t_philo *sophers)
{
	ssize_t	i;

	i = -1;
	while(++i <= sophers->philo_count)
	{
		if (pthread_create(&sophers->philosophers[i], NULL, &start_routine, 
					 (void *)&sophers->ids[i]) != 0)
			return(join_prev_threads(sophers, i), 0);
	}
	return (1);
}
int	loop_infinite(t_philo *sophers)
{
	if (!get_time_atomic(&sophers->atomic_ustime))
		return (0);
	if (!init_threads(sophers))
		return (0);
	while (sophers->start != sophers->philo_count + 1)
		usleep(1);
	usleep(0);
	while (1)
	{
		if (sophers->death != 0)
			break ;
	}
	return (1);
}

int	create_philos(t_philo *sophers)
{

	sophers->philosophers = malloc((sophers->philo_count + 1) * sizeof(pthread_t));
	if (!sophers->philosophers)
		return (0);
	return (1);
}
int	main(int argc, char **argv)
{
	t_philo	sophers;

	memset(sophers.buffer, 0, 2000);
	if (parse_and_store(&sophers, argc, argv) == -1)
		return (1);
	if (!create_forks(&sophers))
		return (1);
	if (!create_states(&sophers))
		return (destroy_prev_forks(&sophers, sophers.philo_count + 1), 1);
	if (!create_philos(&sophers))
		return(destroy_prev_forks(&sophers, sophers.philo_count + 1),  free(sophers.states), 1);
	if (!create_ids(&sophers))
		return (destroy_prev_forks(&sophers, sophers.philo_count + 1),
			free(sophers.states), 1);
	if (sophers.eat_count == NA)
		loop_infinite(&sophers);
	free(sophers.ids);
	free(sophers.states);
	join_prev_threads(&sophers, sophers.philo_count + 1);
	destroy_prev_forks(&sophers, sophers.philo_count + 1);
	
}
