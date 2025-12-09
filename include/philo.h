/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hichikaw <hichikaw@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/27 20:18:57 by ichikawahik       #+#    #+#             */
/*   Updated: 2025/09/30 19:16:51 by hichikaw         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <sys/time.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <stdbool.h>

typedef struct s_config
{
	int	number_of_philosophers;
	int	time_to_die_ms;
	int	time_to_eat_ms;
	int	time_to_sleep_ms;
	int	must_eat_count;
}	t_config;

struct	s_state;

typedef struct s_philo
{
	int				id;
	int				left_fork_index;
	int				right_fork_index;
	long long		last_meal_ms;
	int				meals_eaten;
	pthread_t		thread;
	struct s_state	*state;
}	t_philo;

typedef struct s_state
{
	t_config		config;
	long long		sim_start_ms;
	bool			simulation_end;
	pthread_mutex_t	*forks;
	pthread_mutex_t	print_mutex;
	pthread_mutex_t	state_mutex;
	t_philo			*philos;
	pthread_t		monitor_thread;
}	t_state;

int			parse_args(int argc, char **argv, t_config *config);
int			init_state(t_state *state, const t_config *config);
void		init_philos(t_state *state, int n);
void		*philosopher_routine(void *arg);
void		*monitor_routine(void *arg);
void		log_action(t_state *state, int philo_id,
				const char *action, bool is_death);
int			str_to_int_strict(const char *s, int *out);
long long	now_ms(void);
void		sleep_ms_interruptible(t_state *state, int duration_ms);
void		cleanup_state(t_state *state);

void		take_forks(t_philo *ph);
void		put_forks(t_philo *ph);
void		eat(t_philo *ph);
void		philo_sleep_and_think(t_philo *ph);
bool		is_sim_over(t_state *st);

#endif
