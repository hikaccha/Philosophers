/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 00:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/09/30 20:23:04 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	log_action(t_state *state, int philo_id,
			const char *action, bool is_death)
{
	long long	stamp;

	pthread_mutex_lock(&state->state_mutex);
	if (state->simulation_end && !is_death)
	{
		pthread_mutex_unlock(&state->state_mutex);
		return ;
	}
	stamp = now_ms() - state->sim_start_ms;
	pthread_mutex_unlock(&state->state_mutex);
	pthread_mutex_lock(&state->print_mutex);
	printf("%lld ms: %d %s\n", stamp, philo_id, action);
	pthread_mutex_unlock(&state->print_mutex);
}
