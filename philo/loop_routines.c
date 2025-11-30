/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop_routines.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvcampo <alvcampo@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 16:47:45 by alvcampo          #+#    #+#             */
/*   Updated: 2025/11/30 16:53:11 by alvcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	routine_loop_odds(void *ids)
{
	t_id	*cast_id;

	cast_id = (t_id *)ids;
	while (1)
	{
		if (!think_routine(cast_id) && atomic_load(cast_id->death) != 0)
			return ;
		if (!eat_routine(cast_id) && atomic_load(cast_id->death) != 0)
			return ;
		if (atomic_load(cast_id->eat_count) == cast_id->times_eaten)
			return ;
		if (!sleep_routine(cast_id) && atomic_load(cast_id->death) != 0)
			return ;
		usleep(1);
	}
}

void	routine_loop_last(void *ids)
{
	t_id	*cast_id;

	cast_id = (t_id *)ids;
	while (1)
	{
		if (!sleep_routine(cast_id) && atomic_load(cast_id->death) != 0)
			return ;
		if (!think_routine(cast_id) && atomic_load(cast_id->death) != 0)
			return ;
		if (!eat_routine(cast_id) && atomic_load(cast_id->death) != 0)
			return ;
		if (atomic_load(cast_id->eat_count) == cast_id->times_eaten)
			return ;
		usleep(1);
	}
}

void	routine_loop_even(void *ids)
{
	t_id	*cast_id;

	cast_id = (t_id *)ids;
	while (1)
	{
		if (!eat_routine(cast_id) && atomic_load(cast_id->death) != 0)
			return ;
		if (atomic_load(cast_id->eat_count) == cast_id->times_eaten)
			return ;
		if (!sleep_routine(cast_id) && atomic_load(cast_id->death) != 0)
			return ;
		if (!think_routine(cast_id) && atomic_load(cast_id->death) != 0)
			return ;
		usleep(1);
	}
}
