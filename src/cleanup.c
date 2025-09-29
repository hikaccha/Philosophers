/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 00:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/09/28 19:24:27 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	cleanup_state(t_state *state)
{
	int	idx;
	int	n;

	n = state->config.number_of_philosophers;
	if (state->forks)
	{
		idx = 0;
		while (idx < n)
		{
			pthread_mutex_destroy(&state->forks[idx]);
			idx++;
		}
		free(state->forks);
		state->forks = NULL;
	}
	if (state->philos)
	{
		free(state->philos);
		state->philos = NULL;
	}
	pthread_mutex_destroy(&state->print_mutex);
	pthread_mutex_destroy(&state->state_mutex);
}
