/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 00:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/09/28 17:36:37 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	now_ms(void)
{
	struct timeval		v;

	gettimeofday(&v, NULL);
	return ((long long)v.tv_sec * 1000LL + (v.tv_usec / 1000));
}

void		sleep_ms_interruptible(t_state *state, int duration_ms)
{
	long long	start;
	bool		end;

	start = now_ms();
	while (now_ms() - start < duration_ms)
	{
		pthread_mutex_lock(&state->state_mutex);
		end = state->simulation_end;
		pthread_mutex_unlock(&state->state_mutex);
		if (end)
			break ;
		usleep(1000);
	}
}
