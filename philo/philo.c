#include "philo.h"

int	main(int argc, char **argv)
{
	t_args	args;

	if (parse_and_store(&args, argc, argv) == -1)
		return (1);

}
