#include "philo.h"
int	check_overflow_and_negative(char *str)
{
	size_t	len;

	if (str == NULL)
		return(-1);
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

	i = -1;
	while(argv[++i] != NULL)
	{
		j = 0;
		while(argv[i][j])
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
	while(argv[++i] != NULL)
	{
		if(check_overflow_and_negative(argv[i]) == -1)
			return (-1);
	}
	return (1);
}
int	parse_and_store(t_philo *sophers, int argc, char **argv)
{

	if (argc < 5 || argc > 6)
		return(ft_putstr_fd("Invalid sophers", 2), -1);
	if (contains_only_digits(argv) == -1)
		return(ft_putstr_fd("Only digits allowed", 2), -1);
	if (check_argv_for_ov_neg(argv) == -1)
		return(ft_putstr_fd("Negative or > INT_MAX not allowed", 2), -1);
	sophers->philo_count = ft_atoi(argv[1]);
	if (sophers->philo_count >= 1000)
		return(ft_putstr_fd("Too much philosophers", 2), -1);
	atomic_init(&(sophers->ttd), ft_atoi(argv[2]));
	atomic_init(&(sophers->tte), ft_atoi(argv[2]));
	atomic_init(&(sophers->tts), ft_atoi(argv[2]));
	if (argc == 6)
		sophers->eat_count = ft_atoi(argv[5]);
	if (argc == 5)
		sophers->eat_count = NA;
	return (1);
}
