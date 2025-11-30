/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvcampo <alvcampo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 09:57:36 by alvcampo          #+#    #+#             */
/*   Updated: 2025/11/30 00:21:04 by alvcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	create_states(t_philo *sophers)
{
	atomic_init(&sophers->start, 0);
	atomic_init(&sophers->death, 0);
	atomic_init(&sophers->end, 0);
}

void	*start_routine(void *ids)
{
	t_id				stack_ids;
	atomic_size_t		time;

	ft_memcpy((void *)&stack_ids, ids, sizeof(t_id));
	atomic_fetch_add(stack_ids.start, 1);
	while (atomic_load(stack_ids.start) != stack_ids.atomic_p_count + 1)
		usleep(1);
	stack_ids.local_time = 0;
	while(!stack_ids.local_time)
	{
		stack_ids.local_time = atomic_load(stack_ids.start_time);
		usleep(1);
	}
	get_time_atomic(&time);
	atomic_store(&stack_ids.last_ate, atomic_load(&time));
	// atomic_store(&stack_ids.last_ate, atomic_load(&time));
	if (stack_ids.number == 0)
			test_routine(&stack_ids);
	else if (atomic_load(&stack_ids.atomic_p_count) == 1)
		one_philo_routine(&stack_ids);
	else if (stack_ids.number % 2 == 0)
		routine_loop_even(&stack_ids);
	else if (stack_ids.number != atomic_load(&stack_ids.atomic_p_count))
		routine_loop_odds(&stack_ids);
	else
		routine_loop_last(&stack_ids);
	printf("hehe from %i\n", stack_ids.number);
	return (NULL);
}

int	loop_infinite(t_philo *sophers)
{
	if (!init_threads(sophers))
		return (0);
	if (!get_time_atomic(&sophers->atomic_ustime))
		return (0);
	while (atomic_load(&sophers->start) != sophers->philo_count + 1)
		usleep(10);
	return (1);
}

int	main(int argc, char **argv)
{
	t_philo	sophers;

	if (parse_and_store(&sophers, argc, argv) == -1)
		return (1);
	if (!create_forks(&sophers))
		return (1);
	create_states(&sophers);
	if (!create_philos(&sophers))
		return (destroy_prev_forks(&sophers, sophers.philo_count + 1), 1);
	if (!create_ids(&sophers))
		return (destroy_prev_forks(&sophers, sophers.philo_count + 1), 1);
	loop_infinite(&sophers);
	join_prev_threads(&sophers, sophers.philo_count + 1);
	free(sophers.ids);
	destroy_prev_forks(&sophers, sophers.philo_count + 1);
}
