#include "philo.h"

long long now_ms(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000LL + (tv.tv_usec / 1000);
}

void sleep_ms_interruptible(t_state *state, int duration_ms)
{
    long long start = now_ms();
    while (now_ms() - start < duration_ms)
    {
        pthread_mutex_lock(&state->state_mutex);
        bool end = state->simulation_end;
        pthread_mutex_unlock(&state->state_mutex);
        if (end)
            break;
        usleep(1000);
    }
}


