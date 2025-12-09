/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_philo.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/28 00:00:00 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/09/29 16:51:26 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	assign_forks(t_philo *p, int total)
{
	p->left_fork_index = p->id - 1;
	p->right_fork_index = p->id % total;
}

void	init_philos(t_state *state, int n)
{
	int		idx;
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
