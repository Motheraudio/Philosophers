# Philosophers

A simulation of the classic dining philosophers problem, implemented in C with POSIX threads and mutexes.

## What It Does

This program simulates multiple philosophers sitting at a round table.  Each philosopher needs two forks to eat, but there are only as many forks as there are philosophers.  The program demonstrates concurrent programming concepts and the challenges of resource sharing between threads.

Each philosopher follows a simple routine:
- Think
- Eat (requires picking up two forks)
- Sleep
- Repeat

The simulation ends when a philosopher dies from starvation or when all philosophers have eaten a specified number of times.

## How It Works

### Program Arguments

```
./philo <num_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [times_must_eat]
```

- `num_philosophers`: Number of philosophers (and forks)
- `time_to_die`: Milliseconds until a philosopher dies without eating
- `time_to_eat`: Milliseconds it takes to eat
- `time_to_sleep`: Milliseconds spent sleeping
- `times_must_eat`: (optional) Number of times each philosopher must eat before the simulation ends

### Implementation Details

The program creates one thread per philosopher, plus an extra monitoring thread. Each philosopher is assigned a unique ID and gets access to two forks (mutexes) on their left and right. 

**Key data structures:**

- `t_philo`: Main structure holding simulation parameters, fork mutexes, threads, and shared atomic variables
- `t_id`: Per-philosopher structure with pointers to shared resources and local state

**Synchronization:**

All shared state uses atomic variables to avoid data races: 
- `death`: Tracks if any philosopher has died
- `end`: Tracks if all philosophers have finished eating
- `start`: Used to synchronize thread startup

**Fork acquisition:**

Philosophers pick up forks in a specific order to prevent deadlock.  The last philosopher picks up forks in reverse order compared to the others.

**Death detection:**

Before and after every action, the program checks if a philosopher has exceeded their time to die. The `test_routine` thread monitors for death and completion conditions.

**Different routines for different philosophers:**

- Even-numbered philosophers start by eating
- Odd-numbered philosophers start by thinking
- The last philosopher starts by sleeping

This staggering reduces contention for forks and helps prevent starvation.

**Time handling:**

The program uses `gettimeofday()` to track elapsed time in milliseconds.  A custom `my_usleep()` function provides precise sleep intervals while checking for death conditions.

## Building and Running

Build the program: 
```bash
cd philo
make
```

Run a simulation:
```bash
./philo 5 800 200 200
```

This creates 5 philosophers who will die if they don't eat within 800ms, take 200ms to eat, and sleep for 200ms.

## Files

- `philo.c`: Main program entry and initialization
- `parse. c`: Argument parsing and validation
- `threads.c`: Thread creation and management
- `routines.c`: Core philosopher routines (eat, sleep, think)
- `loop_routines.c`: Different loop patterns for even/odd/last philosophers
- `eat_helpers.c`: Fork acquisition logic
- `create_forks.c`: Mutex initialization
- `create_ids.c`: Philosopher ID structure setup
- `utils.c`: Time and utility functions
- `ft. c`: Basic string/number utilities
- `philo.h`: Header with all structure definitions and function declarations

## Edge Cases

The single philosopher case is handled specially since they can only pick up one fork and will inevitably die. 
