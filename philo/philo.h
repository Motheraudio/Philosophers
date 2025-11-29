/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvcampo <alvcampo@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 10:04:10 by alvcampo          #+#    #+#             */
/*   Updated: 2025/11/24 19:18:15 by alvcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# define NA -1
# define FORK 1
# define EAT 2
# define SLEEP 3
# define THINK 4
# define DIE 5
# define LEFT 1
# define RIGHT 0
# include <stdlib.h>
# include <pthread.h>
# include <stdatomic.h>
# include <unistd.h>
# include <sys/time.h>
# include <string.h>
# include <stdio.h>

typedef struct s_id
{
	atomic_int		number;
	atomic_size_t			last_ate;
	atomic_int		times_eaten;
	atomic_size_t	*start_time;
	atomic_size_t	*tte;
	atomic_size_t	*tts;
	atomic_size_t	*ttd;
	atomic_size_t	*ttt;
	atomic_int		*eat_count;
	pthread_mutex_t	*forks[2];
	atomic_int		*start;
	atomic_int		*death;
	atomic_int		*end;
	pthread_mutex_t	*print_mutex;
	void			*(*start_routine)(void *t_id);
	atomic_int		atomic_p_count;
	size_t			timetothink;

}				t_id;

typedef struct s_philo
{
	int				philo_count;
	atomic_size_t	ttd;
	atomic_size_t	tte;
	atomic_size_t	tts;
	atomic_size_t	ttt;
	atomic_int		eat_count;
	pthread_mutex_t	*forks;
	pthread_t		*philosophers;
	atomic_size_t	atomic_ustime;
	t_id			*ids;
	atomic_int		start;
	atomic_int		death;
	atomic_int		end;
}				t_philo;

					/* Parse */
int		parse_and_store(t_philo *sophers, int argc, char **argv);

					/* Utilities */
size_t	ft_strlen(const char *s);
int		ft_atoi(const char *str);
void	ft_putstr_fd(char *s, int fd);
size_t	ft_strlcpy(char *dst, const char *src, size_t size);
void	*ft_memcpy(void *dst, const void *src, size_t n);
					/* Fork creation */
void	destroy_prev_forks(t_philo *sophers, ssize_t i);
int		create_forks(t_philo *sophers);

					/* ID creation */
int		create_ids(t_philo *sophers);
					/* Routines */
void	*test_routine(void *ids);
void	*think_routine(void *ids);
void	*sleep_routine(void *ids);
void	*eat_routine(void *ids);
void	*start_routine(void *ids);
void	*one_philo_routine(void *ids);
int		pick_forks(t_id *cast_id, atomic_size_t *time);
void	select_fork(t_id *cast_id, pthread_mutex_t **f, pthread_mutex_t **s);
					/* Time */
int		get_time_atomic(atomic_size_t *ustime);
					/* Threads */
int		create_philos(t_philo *sophers);
void	join_prev_threads(t_philo *sophers, ssize_t i);
int		init_threads(t_philo *sophers);
int	my_usleep(t_id *cast_id, size_t ttw);
					/* Print */
void	print_mutex(t_id *ids, char *message, atomic_size_t time);
					/* loops */
void	routine_loop_odds(void *ids);
void	routine_loop_even(void *ids);

#endif
