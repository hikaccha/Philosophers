/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 00:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/09 16:21:22 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	take_forks(t_philo *ph)
{
	t_state	*st;
	int		left;
	int		right;

	st = ph->state;
	left = ph->left_fork_index;
	right = ph->right_fork_index;
	if (ph->id % 2 == 0)
	{
		pthread_mutex_lock(&st->forks[right]);
		log_action(st, ph->id, "has taken a fork", false);
		pthread_mutex_lock(&st->forks[left]);
		log_action(st, ph->id, "has taken a fork", false);
	}
	else
	{
		pthread_mutex_lock(&st->forks[left]);
		log_action(st, ph->id, "has taken a fork", false);
		pthread_mutex_lock(&st->forks[right]);
		log_action(st, ph->id, "has taken a fork", false);
	}
	pthread_mutex_lock(&st->state_mutex);
	ph->last_meal_ms = now_ms();
	pthread_mutex_unlock(&st->state_mutex);
}

void	put_forks(t_philo *ph)
{
	t_state	*st;

	st = ph->state;
	pthread_mutex_unlock(&st->forks[ph->left_fork_index]);
	pthread_mutex_unlock(&st->forks[ph->right_fork_index]);
}

bool	is_sim_over(t_state *st)
{
	bool	end;

	pthread_mutex_lock(&st->state_mutex);
	end = st->simulation_end;
	pthread_mutex_unlock(&st->state_mutex);
	return (end);
}

void	eat(t_philo *ph)
{
	t_state	*st;

	st = ph->state;
	log_action(st, ph->id, "is eating", false);
	sleep_ms_interruptible(st, st->config.time_to_eat_ms);
	pthread_mutex_lock(&st->state_mutex);
	ph->meals_eaten += 1;
	pthread_mutex_unlock(&st->state_mutex);
}

void	philo_sleep_and_think(t_philo *ph)
{
	log_action(ph->state, ph->id, "is sleeping", false);
	sleep_ms_interruptible(ph->state, ph->state->config.time_to_sleep_ms);
	log_action(ph->state, ph->id, "is thinking", false);
}
