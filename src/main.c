#include "philo.h"

static void print_usage(void)
{
    fprintf(stderr, "Usage: ./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [must_eat]\n");
}

int main(int argc, char **argv)
{
    t_config config;
    t_state  state;

    if (argc != 5 && argc != 6)
    {
        print_usage();
        return 1;
    }
    if (parse_args(argc, argv, &config) != 0)
        return 1;
    if (init_state(&state, &config) != 0)
        return 1;

    // Launch philosopher threads
    for (int i = 0; i < state.config.number_of_philosophers; ++i)
    {
        if (pthread_create(&state.philos[i].thread, NULL, philosopher_routine, &state.philos[i]) != 0)
        {
            fprintf(stderr, "Error: pthread_create failed\n");
            state.simulation_end = true;
            break;
        }
    }
    // Launch monitor
    if (pthread_create(&state.monitor_thread, NULL, monitor_routine, &state) != 0)
    {
        fprintf(stderr, "Error: pthread_create (monitor) failed\n");
        state.simulation_end = true;
    }

    // Join
    for (int i = 0; i < state.config.number_of_philosophers; ++i)
        pthread_join(state.philos[i].thread, NULL);
    pthread_join(state.monitor_thread, NULL);

    cleanup_state(&state);
    return 0;
}


