#ifndef PHILO_H
# define PHILO_H
# define NA 1
# include <stdlib.h>
# include <pthread.h>
# include <stdatomic.h>
# include <unistd.h>
# include <sys/time.h>

typedef struct	s_id
{
	atomic_size_t	last_ate;
	atomic_size_t	times_ate;
	
}

typedef struct	s_philo
{
	int				philo_count;
	atomic_size_t	ttd;
	atomic_size_t	tte;
	atomic_size_t	tts;
	int				eat_count;
	pthread_mutex_t	*forks;
	pthread_t		*philosophers;
	atomic_size_t	atomic_mstime;
}				t_philo;


int	parse_and_store(t_philo *sophers, int argc, char **argv);
size_t	ft_strlen(const char *s);
int		ft_atoi(const char *str);
void	ft_putstr_fd(char *s, int fd);
#endif
