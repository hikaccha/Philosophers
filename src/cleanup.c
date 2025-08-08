#include "philo.h"

void cleanup_state(t_state *state)
{
    int n = state->config.number_of_philosophers;
    if (state->forks)
    {
        for (int i = 0; i < n; ++i)
            pthread_mutex_destroy(&state->forks[i]);
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


