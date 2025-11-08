#include "philo.h"
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_atoi(const char *str)
{
	size_t	i;
	int	total;
	int	sign;

	i = 0;
	total = 0;
	sign = 1;
	while (str[i] == ' ' || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '+' || str[i] == '-')
	{
		if (str[i] == '-')
			sign *= -1;
		i++;
	}
	while (str[i] >= '0' && str [i] <= '9')
	{
		total = (total * 10) + (str[i] - '0');
		i++;
	}
	return (total * sign);
}

