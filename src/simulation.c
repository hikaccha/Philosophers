#include "philo.h"

static void take_forks(t_philo *ph)
{
    t_state *st = ph->state;
    int left = ph->left_fork_index;
    int right = ph->right_fork_index;

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

static void put_forks(t_philo *ph)
{
    t_state *st = ph->state;
    pthread_mutex_unlock(&st->forks[ph->left_fork_index]);
    pthread_mutex_unlock(&st->forks[ph->right_fork_index]);
}

static bool is_sim_over(t_state *st)
{
    bool end;
    pthread_mutex_lock(&st->state_mutex);
    end = st->simulation_end;
    pthread_mutex_unlock(&st->state_mutex);
    return end;
}

static void eat(t_philo *ph)
{
    t_state *st = ph->state;
    log_action(st, ph->id, "is eating", false);
    pthread_mutex_lock(&st->state_mutex);
    ph->last_meal_ms = now_ms();
    ph->meals_eaten += 1;
    pthread_mutex_unlock(&st->state_mutex);
    sleep_ms_interruptible(st, st->config.time_to_eat_ms);
}

static void philo_sleep_and_think(t_philo *ph)
{
    log_action(ph->state, ph->id, "is sleeping", false);
    sleep_ms_interruptible(ph->state, ph->state->config.time_to_sleep_ms);
    log_action(ph->state, ph->id, "is thinking", false);
}

void *philosopher_routine(void *arg)
{
    t_philo *ph = (t_philo *)arg;
    t_state *st = ph->state;

    if (st->config.number_of_philosophers == 1)
    {
        // Edge case: only one fork; take it and wait for death
        pthread_mutex_lock(&st->forks[ph->left_fork_index]);
        log_action(st, ph->id, "has taken a fork", false);
        while (!is_sim_over(st))
            usleep(1000);
        pthread_mutex_unlock(&st->forks[ph->left_fork_index]);
        return NULL;
    }

    // Small offset to reduce contention
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
                break;
            }
            pthread_mutex_unlock(&st->state_mutex);
        }
        philo_sleep_and_think(ph);
    }
    return NULL;
}

void *monitor_routine(void *arg)
{
    t_state *st = (t_state *)arg;
    int n = st->config.number_of_philosophers;
    while (1)
    {
        int satisfied = 0;
        for (int i = 0; i < n; ++i)
        {
            pthread_mutex_lock(&st->state_mutex);
            long long last_meal = st->philos[i].last_meal_ms;
            int meals = st->philos[i].meals_eaten;
            pthread_mutex_unlock(&st->state_mutex);

            if (st->config.must_eat_count > 0 && meals >= st->config.must_eat_count)
                satisfied++;

            long long elapsed = now_ms() - last_meal;
            if (elapsed >= st->config.time_to_die_ms)
            {
                pthread_mutex_lock(&st->state_mutex);
                st->simulation_end = true;
                pthread_mutex_unlock(&st->state_mutex);
                log_action(st, st->philos[i].id, "died", true);
                return NULL;
            }
        }
        if (st->config.must_eat_count > 0 && satisfied == n)
        {
            pthread_mutex_lock(&st->state_mutex);
            st->simulation_end = true;
            pthread_mutex_unlock(&st->state_mutex);
            return NULL;
        }
        usleep(1000);
    }
}


