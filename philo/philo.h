#ifndef PHILO_H
# define PHILO_H
# define NA 1
# include <stdlib.h>
# include <pthread.h>
# include <stdatomic.h>
# include <unistd.h>
typedef struct	s_args
{
	int		philo_count;
	atomic_size_t	ttd;
	atomic_size_t	tte;
	atomic_size_t	tts;
	int		eat_count;

}				t_args;
int	parse_and_store(t_args *args, int argc, char **argv);
size_t	ft_strlen(const char *s);
int	ft_atoi(const char *str);
#endif
