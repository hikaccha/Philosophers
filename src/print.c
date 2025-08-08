#include "philo.h"

void log_action(t_state *state, int philo_id, const char *action, bool is_death)
{
    long long ts;

    pthread_mutex_lock(&state->state_mutex);
    if (state->simulation_end && !is_death)
    {
        pthread_mutex_unlock(&state->state_mutex);
        return;
    }
    ts = now_ms() - state->sim_start_ms;
    pthread_mutex_unlock(&state->state_mutex);

    pthread_mutex_lock(&state->print_mutex);
    printf("%lld ms: %d %s\n", ts, philo_id, action);
    pthread_mutex_unlock(&state->print_mutex);
}


