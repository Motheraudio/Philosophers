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
	if (i == 0)
		return ;
	sophers->ids[i].forks[0] = sophers->forks[i];
	if (i == 1)
		sophers->ids[i].forks[1] = sophers->forks[sophers->philo_count];
	else if (i == sophers->philo_count)
		sophers->ids[i].forks[1] = sophers->forks[1];
	else
		sophers->ids[i].forks[1] = sophers->forks[i + 1];

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
		atomic_init(&sophers->ids[i].number, sophers->philo_count);
		itoa_4(sophers->ids[i].name, i);
		atomic_init(&sophers->ids[i].last_ate, 0);
		atomic_init(&sophers->ids[i].times_eaten, 0);
		give_forks(sophers, i);
		sophers->ids[i].buffer = sophers->buffer;
		sophers->ids[i].start = &sophers->start;
		sophers->ids[i].death = &sophers->death;
		sophers->ids[i].state = &sophers->states[i];
		sophers->ids[i].tte = &sophers->tte;
		sophers->ids[i].tts = &sophers->tts;
		sophers->ids[i].ttd = &sophers->ttd;
		sophers->ids[i].eat_count = &sophers->eat_count;
	}
	return (1);
}
