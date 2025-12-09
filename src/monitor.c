/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 00:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/10/17 18:40:28 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	check_one(t_state *st, int idx, int *satisfied)
{
	long long	last_meal;
	int			meals;
	long long	elapsed;

	pthread_mutex_lock(&st->state_mutex);
	last_meal = st->philos[idx].last_meal_ms;
	meals = st->philos[idx].meals_eaten;
	pthread_mutex_unlock(&st->state_mutex);
	if (st->config.must_eat_count > 0
		&& meals >= st->config.must_eat_count)
		(*satisfied)++;
	elapsed = now_ms() - last_meal;
	if (elapsed >= st->config.time_to_die_ms)
	{
		pthread_mutex_lock(&st->state_mutex);
		st->simulation_end = true;
		pthread_mutex_unlock(&st->state_mutex);
		log_action(st, st->philos[idx].id, "died", true);
		return (1);
	}
	return (0);
}

static int	check_all_satisfied(t_state *st, int satisfied, int n)
{
	if (st->config.must_eat_count > 0 && satisfied == n)
	{
		pthread_mutex_lock(&st->state_mutex);
		st->simulation_end = true;
		pthread_mutex_unlock(&st->state_mutex);
		return (1);
	}
	return (0);
}

void	*monitor_routine(void *arg)
{
	t_state	*st;
	int		n;
	int		idx;
	int		satisfied;

	st = (t_state *)arg;
	n = st->config.number_of_philosophers;
	while (1)
	{
		satisfied = 0;
		idx = 0;
		while (idx < n)
		{
			if (check_one(st, idx, &satisfied))
				return (NULL);
			idx++;
		}
		if (check_all_satisfied(st, satisfied, n))
			return (NULL);
		usleep(1000);
	}
}
