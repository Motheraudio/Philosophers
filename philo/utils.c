/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvcampo <alvcampo@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/24 19:10:21 by alvcampo          #+#    #+#             */
/*   Updated: 2025/11/30 00:19:36 by alvcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	atomic_store(ustime, start_time.tv_sec * 1000000 + start_time.tv_usec);
	return (1);
}

size_t	get_time()
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return(time.tv_sec * 1000000 + time.tv_usec);
}

int	my_usleep(t_id *cast_id, size_t ttw)
{
	size_t	time_rn;
	atomic_size_t	i;
	//atomic_size_t	time_s;

	i = 0;
	time_rn = get_time();
	//get_time_atomic(&time_s);
	while(ttw > get_time() - time_rn)
	{
		if (i % 50 == 0)
		{
			if (atomic_load(cast_id->death) > 0)
				return (0);
			if (atomic_load(cast_id->ttd) <= time_rn - cast_id->last_ate)
				return (atomic_fetch_add(cast_id->death, 1),
				print_mutex(cast_id, NULL), 0);
		}
		usleep(50);
		// printf("%i: %zu, %zu, %zu\n", cast_id->number, ttw, time_rn - time_s, atomic_load (cast_id->ttd));
		// exit(1);
		i++;
	}
	return (1);
}
