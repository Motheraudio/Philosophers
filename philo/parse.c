/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alvcampo <alvcampo@student.42vienna.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/09 09:54:27 by alvcampo          #+#    #+#             */
/*   Updated: 2025/11/09 18:55:28 by alvcampo         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_overflow_and_negative(char *str)
{
	size_t	len;

	if (str == NULL)
		return (-1);
	len = ft_strlen(str);
	if (str[0] == '-')
		return (-1);
	if (len > 10)
		return (-1);
	if (len == 10 && str[len - 1] > '7')
		return (-1);
	return (1);
}

int	contains_only_digits(char **argv)
{
	ssize_t	i;
	ssize_t	j;

	i = 0;
	while (argv[++i] != NULL)
	{
		j = 0;
		while (argv[i][j])
		{
			if (argv[i][j] < '0' || argv[i][j] > '9')
				return (-1);
			j++;
		}
	}
	return (1);
}

int	check_argv_for_ov_neg(char **argv)
{
	ssize_t	i;

	i = -1;
	while (argv[++i] != NULL)
	{
		if (check_overflow_and_negative(argv[i]) == -1)
			return (-1);
	}
	return (1);
}

int	parse_and_store(t_philo *sophers, int argc, char **argv)
{
	if (argc < 5 || argc > 6)
		return (ft_putstr_fd("Invalid sophers\n", 2), -1);
	if (contains_only_digits(argv) == -1)
		return (ft_putstr_fd("Only digits allowed\n", 2), -1);
	if (check_argv_for_ov_neg(argv) == -1)
		return (ft_putstr_fd("Negative or > INT_MAX not allowed\n", 2), -1);
	sophers->philo_count = ft_atoi(argv[1]);
	if (sophers->philo_count >= 1000)
		return (ft_putstr_fd("Too much philosophers\n", 2), -1);
	atomic_init(&(sophers->ttd), ft_atoi(argv[2]) * 1000);
	atomic_init(&(sophers->tte), ft_atoi(argv[3]) * 1000);
	atomic_init(&(sophers->tts), ft_atoi(argv[4]) * 1000);
	if (sophers->philo_count % 2 == 0)
		atomic_init(&(sophers->ttt), 1);
	else
		atomic_init(&(sophers->ttt), (sophers->tte * 2 - sophers->tts));
	if (argc == 6)
		atomic_init(&(sophers->eat_count), ft_atoi(argv[5]));
	if (argc == 5)
		sophers->eat_count = NA;
	return (1);
}
