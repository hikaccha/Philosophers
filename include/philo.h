#ifndef PHILO_H
#define PHILO_H

#include <pthread.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct s_config
{
	int number_of_philosophers;
	int time_to_die_ms;
	int time_to_eat_ms;
	int time_to_sleep_ms;
	int must_eat_count; // -1 if not provided
} t_config;

struct s_state;

typedef struct s_philo
{
	int id;					// 1..n
	int left_fork_index;	// index in forks array
	int right_fork_index;	// index in forks array
	long long last_meal_ms; // absolute ms since epoch
	int meals_eaten;
	pthread_t thread;
	struct s_state *state;
} t_philo;

typedef struct s_state
{
	t_config config;
	long long sim_start_ms;
	bool simulation_end;

	pthread_mutex_t *forks;		 // size n
	pthread_mutex_t print_mutex; // serialize output
	pthread_mutex_t state_mutex; // protects shared state and philo fields

	t_philo *philos; // size n
	pthread_t monitor_thread;
} t_state;

// init.c
int parse_args(int argc, char **argv, t_config *config);
int init_state(t_state *state, const t_config *config);

// simulation.c
void *philosopher_routine(void *arg);
void *monitor_routine(void *arg);

// print.c
void log_action(t_state *state, int philo_id, const char *action, bool is_death);

// utils.c
int str_to_int_strict(const char *s, int *out);
long long now_ms(void);
void sleep_ms_interruptible(t_state *state, int duration_ms);

// cleanup.c
void cleanup_state(t_state *state);

#endif // PHILO_H
