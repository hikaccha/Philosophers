#include "philo.h"

int str_to_int_strict(const char *s, int *out)
{
    long val = 0;
    int i = 0;
    if (!s || !*s)
        return -1;
    while (s[i])
    {
        if (s[i] < '0' || s[i] > '9')
            return -1;
        val = val * 10 + (s[i] - '0');
        if (val > 2147483647L)
            return -1;
        i++;
    }
    *out = (int)val;
    return 0;
}

int parse_args(int argc, char **argv, t_config *config)
{
    int n, die, eat, sleep, must = -1;
    if (str_to_int_strict(argv[1], &n) || str_to_int_strict(argv[2], &die)
        || str_to_int_strict(argv[3], &eat) || str_to_int_strict(argv[4], &sleep))
    {
        fprintf(stderr, "Error: arguments must be positive integers within int range\n");
        return -1;
    }
    if (argc == 6)
    {
        if (str_to_int_strict(argv[5], &must))
        {
            fprintf(stderr, "Error: must_eat must be a positive integer\n");
            return -1;
        }
    }
    if (n <= 0 || die <= 0 || eat <= 0 || sleep <= 0 || (argc == 6 && must <= 0))
    {
        fprintf(stderr, "Error: non-positive values are not allowed\n");
        return -1;
    }
    config->number_of_philosophers = n;
    config->time_to_die_ms = die;
    config->time_to_eat_ms = eat;
    config->time_to_sleep_ms = sleep;
    config->must_eat_count = (argc == 6) ? must : -1;
    return 0;
}

static void assign_forks(t_philo *p, int total)
{
    p->left_fork_index = p->id - 1;
    p->right_fork_index = p->id % total;
}

int init_state(t_state *state, const t_config *config)
{
    state->config = *config;
    state->simulation_end = false;
    state->forks = NULL;
    state->philos = NULL;
    if (pthread_mutex_init(&state->print_mutex, NULL) != 0)
        return fprintf(stderr, "Error: mutex init failed\n"), -1;
    if (pthread_mutex_init(&state->state_mutex, NULL) != 0)
        return fprintf(stderr, "Error: mutex init failed\n"), -1;

    int n = config->number_of_philosophers;
    state->forks = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * n);
    state->philos = (t_philo *)malloc(sizeof(t_philo) * n);
    if (!state->forks || !state->philos)
        return fprintf(stderr, "Error: malloc failed\n"), -1;
    for (int i = 0; i < n; ++i)
    {
        if (pthread_mutex_init(&state->forks[i], NULL) != 0)
            return fprintf(stderr, "Error: fork mutex init failed\n"), -1;
    }
    state->sim_start_ms = now_ms();
    for (int i = 0; i < n; ++i)
    {
        t_philo *p = &state->philos[i];
        p->id = i + 1;
        p->state = state;
        p->meals_eaten = 0;
        p->last_meal_ms = state->sim_start_ms;
        assign_forks(p, n);
    }
    return 0;
}


