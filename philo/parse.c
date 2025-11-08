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
int	parse_and_store(t_args *args, int argc, char **argv)
{

	if (argc < 5 || argc > 6)
		return(write(2, "Invalid args", 12), -1);
	if (contains_only_digits(argv) == -1)
		return(write(2, "Only digits allowed", 19), -1);
	if (check_argv_for_ov_neg(argv) == -1)
		return (write(2, "Negative or > INT_MAX not allowed", 33), -1);
	args->philo_count = ft_atoi(argv[1]);
	atomic_init(&(args->ttd), ft_atoi(argv[2]));
	atomic_init(&(args->tte), ft_atoi(argv[2]));
	atomic_init(&(args->tts), ft_atoi(argv[2]));
	if (argc == 6)
		args->eat_count = ft_atoi(argv[5]);
	if (argc == 5)
		args->eat_count = NA;
	return (1);
}
