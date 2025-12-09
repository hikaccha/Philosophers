/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 00:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/09 16:21:20 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

static int	init_state_alloc(t_state *state, int n)
{
	if (alloc_arrays(state, n) != 0)
	{
		printf("Error: malloc failed\n");
		return (-1);
	}
	if (init_forks(state, n) != 0)
	{
		printf("Error: fork mutex init failed\n");
		return (-1);
	}
	return (0);
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
		printf("Error: mutex init failed\n");
		return (-1);
	}
	n = config->number_of_philosophers;
	if (init_state_alloc(state, n) != 0)
		return (-1);
	state->sim_start_ms = now_ms();
	init_philos(state, n);
	return (0);
}
