/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ichikawahikaru <ichikawahikaru@student.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 00:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/09/28 19:30:46 by ichikawahik      ###   ########.fr       */
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
	pthread_mutex_lock(&st->state_mutex);
	ph->last_meal_ms = now_ms();
	ph->meals_eaten += 1;
	pthread_mutex_unlock(&st->state_mutex);
	sleep_ms_interruptible(st, st->config.time_to_eat_ms);
}

void	philo_sleep_and_think(t_philo *ph)
{
	log_action(ph->state, ph->id, "is sleeping", false);
	sleep_ms_interruptible(ph->state, ph->state->config.time_to_sleep_ms);
	log_action(ph->state, ph->id, "is thinking", false);
}

static void one_philo_edge(t_philo *ph)
{
	t_state	*st;

	st = ph->state;
	pthread_mutex_lock(&st->forks[ph->left_fork_index]);
	log_action(st, ph->id, "has taken a fork", false);
	while (!is_sim_over(st))
		usleep(1000);
	pthread_mutex_unlock(&st->forks[ph->left_fork_index]);
}

void	*philosopher_routine(void *arg)
{
	t_philo	*ph;
	t_state	*st;

	ph = (t_philo *)arg;
	st = ph->state;
	if (st->config.number_of_philosophers == 1)
	{
		one_philo_edge(ph);
		return (NULL);
	}
	if (ph->id % 2 == 0)
		usleep(1000 * (st->config.time_to_eat_ms / 2));
	while (!is_sim_over(st))
	{
		take_forks(ph);
		eat(ph);
		put_forks(ph);
		if (st->config.must_eat_count > 0)
		{
			pthread_mutex_lock(&st->state_mutex);
			if (ph->meals_eaten >= st->config.must_eat_count)
			{
				pthread_mutex_unlock(&st->state_mutex);
				break ;
			}
			pthread_mutex_unlock(&st->state_mutex);
		}
		philo_sleep_and_think(ph);
	}
	return (NULL);
}

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
		if (st->config.must_eat_count > 0 && satisfied == n)
		{
			pthread_mutex_lock(&st->state_mutex);
			st->simulation_end = true;
			pthread_mutex_unlock(&st->state_mutex);
			return (NULL);
		}
		usleep(1000);
	}
}
