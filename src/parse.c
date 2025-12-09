/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 00:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/09/29 16:51:26 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	str_to_int_strict(const char *s, int *out)
{
	long	val;
	int		i;

	val = 0;
	i = 0;
	if (!s || !*s)
		return (-1);
	while (s[i])
	{
		if (s[i] < '0' || s[i] > '9')
			return (-1);
		val = val * 10 + (s[i] - '0');
		if (val > 2147483647L)
			return (-1);
		i++;
	}
	*out = (int)val;
	return (0);
}

static int	read_main_args(char **argv, t_config *config)
{
	if (str_to_int_strict(argv[1], &config->number_of_philosophers))
		return (-1);
	if (str_to_int_strict(argv[2], &config->time_to_die_ms))
		return (-1);
	if (str_to_int_strict(argv[3], &config->time_to_eat_ms))
		return (-1);
	if (str_to_int_strict(argv[4], &config->time_to_sleep_ms))
		return (-1);
	return (0);
}

static int	read_optional_arg(int argc, char **argv, t_config *config)
{
	if (argc == 6)
	{
		if (str_to_int_strict(argv[5], &config->must_eat_count))
			return (-1);
	}
	else
		config->must_eat_count = -1;
	return (0);
}

static int	validate_config(int argc, t_config *config)
{
	if (config->number_of_philosophers <= 0)
		return (-1);
	if (config->time_to_die_ms <= 0)
		return (-1);
	if (config->time_to_eat_ms <= 0)
		return (-1);
	if (config->time_to_sleep_ms <= 0)
		return (-1);
	if (argc == 6 && config->must_eat_count <= 0)
		return (-1);
	return (0);
}

int	parse_args(int argc, char **argv, t_config *config)
{
	if (read_main_args(argv, config) != 0
		|| read_optional_arg(argc, argv, config) != 0)
	{
		printf("Error: arguments must be positive integers\n");
		return (-1);
	}
	if (validate_config(argc, config) != 0)
	{
		printf("Error: non-positive values not allowed\n");
		return (-1);
	}
	return (0);
}
