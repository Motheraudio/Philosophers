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
int	get_time_start(t_id *id)
{
	struct timeval	curr_time;

	if (gettimeofday(&curr_time, NULL) == -1)
		return (ft_putstr_fd("how did u break gettimeofday", 2), 0); // free and kill mutexes!
	atomic_init(id->start_time,
		curr_time.tv_sec * 1000000 + curr_time.tv_usec); // this is in microseconds, for printing ineed miliseconds
	return (1);
}


