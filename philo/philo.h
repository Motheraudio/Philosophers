/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvcampo <alvcampo@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 10:04:10 by alvcampo          #+#    #+#             */
/*   Updated: 2025/11/09 18:38:35 by alvcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# define NA 1
# define ZZ 2
# define RIP 3
# include <stdlib.h>
# include <pthread.h>
# include <stdatomic.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>

typedef struct s_id
{
	atomic_int		number;
	char			name[4];
	atomic_size_t	last_ate;
	atomic_int		times_eaten;
	atomic_size_t	*tte;
	atomic_size_t	*tts;
	atomic_size_t	*ttd;
	atomic_int		*eat_count;
	pthread_mutex_t forks[2];
	char			*buffer;
	atomic_int		*state;
	atomic_int		*start;
	atomic_int		*death;
}				t_id;

typedef struct s_philo
{
	int				philo_count;
	atomic_size_t	ttd;
	atomic_size_t	tte;
	atomic_size_t	tts;
	atomic_int		eat_count;
	pthread_mutex_t	*forks;
	pthread_t		*philosophers;
	atomic_size_t	atomic_ustime;
	t_id			*ids;
	char			buffer[2000];
	atomic_int		*states;
	atomic_int		start;
	atomic_int		death;
}				t_philo;

					/* Parse */
int		parse_and_store(t_philo *sophers, int argc, char **argv);

					/* Utilities */
size_t	ft_strlen(const char *s);
int		ft_atoi(const char *str);
void	ft_putstr_fd(char *s, int fd);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
					/* Fork creation */
void	destroy_prev_forks(t_philo *sophers, ssize_t i);
int		create_forks(t_philo *sophers);

					/* ID creation */
int		create_ids(t_philo *sophers);

#endif
