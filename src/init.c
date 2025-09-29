/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 00:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/09/28 17:36:34 by ichikawahik      ###   ########.fr       */
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

static int	read_main_args(char **argv, int *n, int *die, int *eat, int *sleep)
{
	if (str_to_int_strict(argv[1], n))
		return (-1);
	if (str_to_int_strict(argv[2], die))
		return (-1);
	if (str_to_int_strict(argv[3], eat))
		return (-1);
	if (str_to_int_strict(argv[4], sleep))
		return (-1);
	return (0);
}

static int	read_optional_arg(int argc, char **argv, int *must)
{
	if (argc == 6)
	{
		if (str_to_int_strict(argv[5], must))
			return (-1);
	}
	else
		*must = -1;
	return (0);
}

int	parse_args(int argc, char **argv, t_config *config)
{
	int	n;
	int	die;
	int	eat;
	int	sleep;
	int	must;

	if (read_main_args(argv, &n, &die, &eat, &sleep) != 0
		|| read_optional_arg(argc, argv, &must) != 0)
	{
		fprintf(stderr, "Error: arguments must be positive integers\n");
		return (-1);
	}
	if (n <= 0 || die <= 0 || eat <= 0 || sleep <= 0
		|| (argc == 6 && must <= 0))
	{
		fprintf(stderr, "Error: non-positive values not allowed\n");
		return (-1);
	}
	config->number_of_philosophers = n;
	config->time_to_die_ms = die;
	config->time_to_eat_ms = eat;
	config->time_to_sleep_ms = sleep;
	config->must_eat_count = must;
	return (0);
}

static void	assign_forks(t_philo *p, int total)
{
	p->left_fork_index = p->id - 1;
	p->right_fork_index = p->id % total;
}

static int	init_mutexes(t_state *state)
{
	if (pthread_mutex_init(&state->print_mutex, NULL) != 0)
		return (-1);
	if (pthread_mutex_init(&state->state_mutex, NULL) != 0)
		return (-1);
	return (0);
}

static int	alloc_arrays(t_state *state, int n)
{
	state->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * n);
	state->philos = (t_philo *)malloc(sizeof(t_philo) * n);
	if (!state->forks || !state->philos)
		return (-1);
	return (0);
}

static int	init_forks(t_state *state, int n)
{
	int	idx;

	idx = 0;
	while (idx < n)
	{
		if (pthread_mutex_init(&state->forks[idx], NULL) != 0)
			return (-1);
		idx++;
	}
	return (0);
}

static void	init_philos(t_state *state, int n)
{
	int	idx;
	t_philo	*p;

	idx = 0;
	while (idx < n)
	{
		p = &state->philos[idx];
		p->id = idx + 1;
		p->state = state;
		p->meals_eaten = 0;
		p->last_meal_ms = state->sim_start_ms;
		assign_forks(p, n);
		idx++;
	}
}

int	init_state(t_state *state, const t_config *config)
{
	int	n;

	state->config = *config;
	state->simulation_end = false;
	state->forks = NULL;
	state->philos = NULL;
	if (init_mutexes(state) != 0)
	{
		fprintf(stderr, "Error: mutex init failed\n");
		return (-1);
	}
	n = config->number_of_philosophers;
	if (alloc_arrays(state, n) != 0)
	{
		fprintf(stderr, "Error: malloc failed\n");
		return (-1);
	}
	if (init_forks(state, n) != 0)
	{
		fprintf(stderr, "Error: fork mutex init failed\n");
		return (-1);
	}
	state->sim_start_ms = now_ms();
	init_philos(state, n);
	return (0);
}
