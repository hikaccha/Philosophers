/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 00:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/12/09 18:17:39 by ichikawahik      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	one_philo_edge(t_philo *ph)
{
	t_state	*st;

	st = ph->state;
	pthread_mutex_lock(&st->state_mutex);
	ph->last_meal_ms = now_ms();
	pthread_mutex_unlock(&st->state_mutex);
	pthread_mutex_lock(&st->forks[ph->left_fork_index]);
	log_action(st, ph->id, "has taken a fork", false);
	while (!is_sim_over(st))
		usleep(1000);
	pthread_mutex_unlock(&st->forks[ph->left_fork_index]);
}

static int	check_meals_done(t_philo *ph, t_state *st)
{
	int	done;

	done = 0;
	if (st->config.must_eat_count > 0)
	{
		pthread_mutex_lock(&st->state_mutex);
		if (ph->meals_eaten >= st->config.must_eat_count)
			done = 1;
		pthread_mutex_unlock(&st->state_mutex);
	}
	return (done);
}

void	philo_sleep_and_think(t_philo *ph)
{
	t_state	*st;

	st = ph->state;
	log_action(st, ph->id, "is sleeping", false);
	sleep_ms_interruptible(st, st->config.time_to_sleep_ms);
	log_action(st, ph->id, "is thinking", false);
	if (st->config.number_of_philosophers % 2 == 1)
		usleep(500);
}

static void	philo_loop(t_philo *ph, t_state *st)
{
	while (!is_sim_over(st))
	{
		take_forks(ph);
		eat(ph);
		put_forks(ph);
		if (check_meals_done(ph, st))
			break ;
		philo_sleep_and_think(ph);
	}
}

void	*philosopher_routine(void *arg)
{
	t_philo	*ph;
	t_state	*st;

	ph = (t_philo *)arg;
	st = ph->state;
	if (st->config.number_of_philosophers == 1)
		return (one_philo_edge(ph), NULL);
	if (ph->id % 2 == 0)
		usleep(st->config.time_to_eat_ms * 500 + 100);
	pthread_mutex_lock(&st->state_mutex);
	ph->last_meal_ms = now_ms();
	pthread_mutex_unlock(&st->state_mutex);
	philo_loop(ph, st);
	return (NULL);
}
