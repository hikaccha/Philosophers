/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 00:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/09/28 19:26:33 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	print_usage(void)
{
	fprintf(stderr,
		"Usage: ./philo number_of_philosophers time_to_die time_to_eat "
		"time_to_sleep [must_eat]\n");
}

static int	launch_philos(t_state *state)
{
	int	idx;

	idx = 0;
	while (idx < state->config.number_of_philosophers)
	{
		if (pthread_create(&state->philos[idx].thread, NULL,
				philosopher_routine, &state->philos[idx]) != 0)
		{
			fprintf(stderr, "Error: pthread_create failed\n");
			pthread_mutex_lock(&state->state_mutex);
			state->simulation_end = true;
			pthread_mutex_unlock(&state->state_mutex);
			return (1);
		}
		idx++;
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_config	config;
	t_state		state;
	int			idx;

	if (argc != 5 && argc != 6)
	{
		print_usage();
		return (1);
	}
	if (parse_args(argc, argv, &config) != 0)
		return (1);
	if (init_state(&state, &config) != 0)
		return (1);
	if (launch_philos(&state) != 0)
	{
		cleanup_state(&state);
		return (1);
	}
	if (pthread_create(&state.monitor_thread, NULL,
			monitor_routine, &state) != 0)
	{
		fprintf(stderr, "Error: pthread_create (monitor) failed\n");
		pthread_mutex_lock(&state.state_mutex);
		state.simulation_end = true;
		pthread_mutex_unlock(&state.state_mutex);
	}
	idx = 0;
	while (idx < state.config.number_of_philosophers)
	{
		pthread_join(state.philos[idx].thread, NULL);
		idx++;
	}
	pthread_join(state.monitor_thread, NULL);
	cleanup_state(&state);
	return (0);
}
