# Philosophers

A solution to the classic Dining Philosophers problem using POSIX threads and mutexes.

## Problem Description

The Dining Philosophers problem is a classic synchronization problem in computer science. Five philosophers sit around a table with five forks. Each philosopher needs two forks to eat. The challenge is to design a concurrent algorithm such that no philosopher starves.

## Features

- Multi-threaded simulation using POSIX threads
- Deadlock prevention through ordered resource acquisition
- Death monitoring with configurable time limits
- Optional meal count termination
- Thread-safe logging system
- Proper resource cleanup

## Compilation

```bash
make
```

## Usage

```bash
./philo number_of_philosophers time_to_die time_to_eat time_to_sleep [must_eat]
```

### Parameters

- `number_of_philosophers`: Number of philosophers (and forks)
- `time_to_die`: Time in milliseconds after which a philosopher dies if they haven't eaten
- `time_to_eat`: Time in milliseconds it takes for a philosopher to eat
- `time_to_sleep`: Time in milliseconds a philosopher spends sleeping
- `must_eat` (optional): Number of times each philosopher must eat before the simulation ends

### Example

```bash
# 5 philosophers, die after 800ms without eating, eat for 200ms, sleep for 200ms
./philo 5 800 200 200

# Same as above, but terminate after each philosopher eats 3 times
./philo 5 800 200 200 3
```

## Implementation Details

- Even-numbered philosophers pick up the right fork first to prevent deadlock
- Odd-numbered philosophers pick up the left fork first
- A monitor thread continuously checks for deaths and termination conditions
- All shared state is protected by mutexes
- Timestamps are relative to simulation start time

## Cleanup

```bash
make fclean
```