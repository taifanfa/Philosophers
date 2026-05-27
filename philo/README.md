*This project has been created as part of the 42 curriculum by tmorais-.*

# 🍝 Philosophers

## Description

The **Philosophers** project is a classic concurrency problem from computer science, based on Edsger Dijkstra's *Dining Philosophers Problem*. The goal is to simulate a group of philosophers sitting around a round table, each alternating between **eating**, **sleeping**, and **thinking** — while sharing a limited number of forks.

The challenge lies in preventing:
- **Deadlock** — where philosophers wait forever for forks that will never be available.
- **Starvation** — where a philosopher never gets to eat and eventually dies.
- **Data races** — where multiple threads access shared data concurrently without proper synchronization.

This mandatory part implements the simulation using **POSIX threads (pthreads)** and **mutexes**.

---

## Instructions

### Requirements

- GCC or compatible C compiler (`cc`)
- POSIX threads support (`-pthread`)
- Linux or macOS

### Compilation

```bash
cd philo
make
```

This will compile the program with the flags `-Wall -Wextra -Werror -pthread` and produce the `philo` executable.

### Usage

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> [number_of_times_each_philosopher_must_eat]
```

| Argument | Description |
|---|---|
| `number_of_philosophers` | Number of philosophers (and forks) at the table |
| `time_to_die` (ms) | Time a philosopher can go without eating before dying |
| `time_to_eat` (ms) | Time it takes for a philosopher to eat (holds 2 forks) |
| `time_to_sleep` (ms) | Time a philosopher spends sleeping |
| `number_of_times_each_philosopher_must_eat` | *(Optional)* Stops the simulation once all philosophers have eaten this many times |

### Examples

```bash
# 5 philosophers, should run indefinitely without anyone dying
./philo 5 800 200 200

# 4 philosophers, each must eat 7 times, then simulation ends
./philo 4 410 200 200 7

# 1 philosopher — will die (only 1 fork available)
./philo 1 800 200 200
```

### Cleanup

```bash
make clean    # Remove object files
make fclean   # Remove object files and binary
make re       # Full recompile
```

---

## Technical Choices

### Deadlock Prevention — Lock Ordering

The main risk with the Dining Philosophers is **deadlock**: if every philosopher picks up their left fork simultaneously, none can pick up the right fork, and the program hangs forever.

This implementation solves it via **resource ordering**: forks are always acquired in a consistent order (lowest memory address first). This breaks the circular-wait condition, making deadlock structurally impossible regardless of thread scheduling.

```c
if (philo->left_fork < philo->right_fork)
{
    first = philo->left_fork;
    second = philo->right_fork;
}
else
{
    first = philo->right_fork;
    second = philo->left_fork;
}
```

### Thread Synchronization

| Mutex | Purpose |
|---|---|
| `forks[i]` | One per fork — prevents two philosophers from holding the same fork |
| `print_mutex` | Ensures log messages are never interleaved |
| `death_mutex` | Guards the `simulation_over` flag for safe reads/writes across threads |
| `meal_mutex` (per philo) | Protects `last_meal_time` and `meals_eaten` from concurrent access |

### Monitor Thread

A dedicated **monitor thread** runs alongside the philosopher threads. It periodically checks whether any philosopher has exceeded `time_to_die` since their last meal, and whether all philosophers have eaten enough times. This separates the death-detection logic from the philosopher logic, keeping each thread focused on a single responsibility.

### Even/Odd Staggering

Even-numbered philosophers are delayed at the start by `time_to_eat` milliseconds. This staggering reduces fork contention at launch, giving odd philosophers a head start and minimizing the risk of early starvation.

### Precision Sleeping

`usleep()` alone is not precise enough for this project. `ft_usleep()` uses a busy-wait loop with 100µs intervals, checking both elapsed time and the `simulation_over` flag — allowing threads to exit quickly when the simulation ends.

---

## Resources

### Documentation & References

- [POSIX Threads Programming — Lawrence Livermore National Laboratory](https://hpc-tutorials.llnl.gov/posix/)
- [`pthread_mutex_lock` man page](https://man7.org/linux/man-pages/man3/pthread_mutex_lock.3p.html)
- [`gettimeofday` man page](https://man7.org/linux/man-pages/man2/gettimeofday.2.html)
- [Dining Philosophers Problem — Wikipedia](https://en.wikipedia.org/wiki/Dining_philosophers_problem)
- [Coffman Conditions for Deadlock — Wikipedia](https://en.wikipedia.org/wiki/Deadlock#Necessary_conditions)

### AI Usage

AI (Claude) was used during this project for the following:

- **Explaining concepts**: Clarifying the Coffman conditions for deadlock and how lock ordering addresses circular-wait.
- **Debugging assistance**: Helping identify potential data races by reviewing mutex coverage across shared fields.
- **README drafting**: Generating the structure and content of this README.

All AI-generated content was reviewed, tested, and fully understood before being included in the project.
