# Codexion

## Description

Codexion is a concurrent simulation written in C using POSIX threads (`pthreads`).

The simulation represents a circular hub containing a number of coders and dongles. Each coder is represented by a dedicated thread and requires two adjacent dongles in order to compile.

A coder repeatedly performs the following cycle:

1. Acquires both required dongles.
2. Compiles.
3. Releases the dongles.
4. Debugs.
5. Refactors.
6. Requests the dongles again.

Each coder must start compiling within `time_to_burnout` milliseconds of the beginning of its previous compilation. If this deadline is missed, the coder burns out and the simulation stops.

Dongles also have a cooldown period after being released. During this period they cannot be acquired by another coder.

The simulation supports two scheduling policies:

- **FIFO (First In, First Out):** each dongle maintains a queue of waiting coders, and a coder is granted access only when it has priority in both of its required dongle queues. This preserves the FIFO ordering of competing requests for each dongle.
- **EDF (Earliest Deadline First):** each dongle maintains a queue ordered by the deadlines of waiting coders. A coder can proceed only when it has priority in both of its required dongle queues. Equal deadlines are resolved using a deterministic tie-breaking mechanism.

The simulation terminates when either:

- a coder burns out; or
- all coders have completed the required number of compilations.

A dedicated monitor thread is responsible for checking coder deadlines and detecting burnout.

---

## Instructions

### Compilation

Compile the project using:

```bash
make
```

This produces the `codexion` executable.

### Usage

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required dongle_cooldown scheduler
```

Where:

| Argument | Description |
|---|---|
| `number_of_coders` | Number of coder threads and dongles in the simulation |
| `time_to_burnout` | Maximum time in milliseconds between compilation starts |
| `time_to_compile` | Duration of a compilation |
| `time_to_debug` | Duration of debugging |
| `time_to_refactor` | Duration of refactoring |
| `number_of_compiles_required` | Number of compilations required for each coder |
| `dongle_cooldown` | Time in milliseconds before a released dongle becomes available again |
| `scheduler` | Scheduling policy: `fifo` or `edf` |

### Example

```bash
./codexion 5 3000 200 200 200 10 400 fifo
```

or:

```bash
./codexion 5 3000 200 200 200 10 400 edf
```

The program prints timestamped events describing coder activity, including dongle acquisition, compilation, debugging, refactoring, and burnout.

---

## Resources

The project was implemented using the following resources:

- POSIX threads (`pthread_create`, `pthread_join`)
- POSIX mutexes (`pthread_mutex_t`)
- POSIX condition variables (`pthread_cond_t`)
- `pthread_cond_timedwait` for waiting until coder deadlines or other relevant events
- `gettimeofday` for simulation timing
- Standard C memory management (`malloc`, `free`)
- Custom queue-based arbitration for FIFO and EDF scheduling

### Testing and debugging

Several tools were used to verify the implementation:

- **Valgrind** — memory-leak detection and general memory checking.
- **DRD** — detection of threading-related errors.
- **ThreadSanitizer (TSan)** — detection of data races.
- **Helgrind** — additional pthread synchronization analysis.
- **GDB** — debugging segmentation faults and thread behaviour.

### References

Throughout the process of building this project, I used a number of available resources, including:

- YouTube — CodeVault's course on Unix Threads in C.
- GeeksForGeeks — articles about threads and implementation of priority queues in C.
- Wikipedia — page about the Dining Philosophers problem.
- https://medium.com/@yassinx4002/dining-philosophers-in-c-from-theory-to-practice-28582180aa37

### AI usage

AI was used as a learning and debugging aid throughout the project. It was mainly used to:
- clarify concepts related to POSIX threads, mutexes, condition variables, scheduling and concurrency;
- discuss and reason about the project architecture and synchronization strategy before and during implementation;
- analyse compiler, Valgrind, Helgrind, DRD and TSan output and help identify possible sources of errors;
- investigate edge cases and design additional stress tests for the FIFO and EDF schedulers;
- review specific pieces of code for possible race conditions, deadlocks, missed wake-ups and resource-management issues;
- help interpret unexpected behaviour and compare possible implementation approaches;
- assist with documentation, including the structure and wording of this README.

The architecture and implementation were developed by me. AI was used for conceptual explanations, debugging discussions, analysis of tool output, and review of specific implementation choices. It was not used to generate code or to provide a ready-made solution.

### Helgrind note

## Helgrind warning with `pthread_cond_timedwait`

When running the program with Helgrind, a small number of runs may report a warning related to `pthread_cond_signal`/`pthread_cond_broadcast()` and the associated mutex.

The condition variables in the program follow the standard POSIX pattern: shared state is checked while holding the associated mutex, and `pthread_cond_timedwait()` is called with that mutex. Signalling/broadcasting is performed according to the synchronization requirements of the corresponding shared state.

The warning appears intermittently and has not been reproduced as a data race or synchronization error by ThreadSanitizer or DRD. The program also passes the project's functional and stress tests.

Helgrind's own documentation notes that its handling of POSIX condition variables is only partial and that this can result in false-positive reports when synchronization events are not visible to its analysis.
Reference: https://valgrind.org/docs/manual/hg-manual.html/cl-manual.html?utm_source=chatgpt.com

---

## Blocking cases handled

The implementation explicitly handles the main situations that can prevent a coder from progressing.

### 1. Dongles currently unavailable

A coder waits when either of its required dongles:

- is currently being used by another coder; or
- is still in its cooldown period.

Coders do not proceed until both required dongles are available and the coder has priority.

### 2. Arbitration

Each dongle maintains a queue of coders waiting to use it. Because each coder requires two adjacent dongles, a request is granted only when the coder has priority in both relevant queues and both dongles are currently available.

Under FIFO, requests are ordered by arrival. Under EDF, requests are ordered by their compilation deadlines. When EDF deadlines are equal, a deterministic tie-breaking mechanism is used to prevent the ordering from depending solely on thread scheduling.

Once a coder is granted both dongles, they are assigned together before the coder begins compiling.

### 3. Dongle cooldown

After a coder releases its dongles, each dongle remains unavailable until its cooldown expires.

Waiting coders use a timed condition-variable wait so that they can be woken either when the relevant state changes or when the cooldown/deadline needs to be reconsidered.

### 4. Coder burnout

A dedicated monitor thread calculates the next coder deadline and waits until the relevant deadline.

When a deadline is exceeded, the monitor:

1. identifies the coder that burned out;
2. logs the event;
3. stops the simulation;
4. wakes coders waiting for dongles.

### 5. Simulation completion

The simulation also stops when every coder has completed the required number of compilations.

The monitor checks this condition alongside burnout detection so that the simulation can terminate cleanly without waiting for another burnout deadline.

### 6. Thread creation failure

Thread creation errors are handled explicitly.

If creation of a coder thread fails after previous threads have already been created, the simulation is stopped, waiting threads are woken, and all successfully created threads are joined before returning the error.

If the monitor thread cannot be created, the coder threads are stopped and joined before the program exits.

---

## Thread synchronization mechanisms

Because multiple threads access shared simulation state concurrently, access to shared resources is protected using mutexes and condition variables.

### Mutexes

Separate mutexes are used for different categories of shared state:

- `mutex_dongles` protects dongle state and the associated waiting queues.
- `mutex_print` serializes log output so that messages from different threads cannot overlap.
- `mutex_compiles` protects compilation-related coder state, including compilation counters and compilation timestamps.
- `mutex_sim` protects shares simulation state such as the stop flag.
- `mutex_monitor` protects the monitor's condition-variable waiting operation.

Using separate mutexes limits contention and makes the ownership of shared state explicit.

### Condition variables

Two condition variables are used:

- `cond_dongles` wakes coders waiting for dongles.
- `cond_monitor` wakes the monitor when simulation state changes or when it needs to re-evaluate its deadline.

Coders wait using `pthread_cond_timedwait`, allowing them to wake when a relevant condition changes while also ensuring that a cooldown/deadline does not cause them to sleep indefinitely.

Condition variables are always used together with their associated mutex and the relevant state is re-checked after waking. This accounts for spurious wake-ups and for the possibility that another thread may have changed the state before the waiting thread reacquires the mutex.

### Simulation termination

The shared `stop_simulation` flag is protected by `mutex_sim`.

When the simulation must stop, the flag is set and waiting threads are notified through the relevant condition variable. This allows the program to terminate without cancelling threads or leaving threads blocked indefinitely.

### Logging

All output is serialized using `mutex_print`, ensuring that concurrent coder and monitor threads cannot interleave their log messages.

### Thread lifecycle

Coder threads are created dynamically and receive their corresponding coder structure as their thread argument.

A separate monitor thread observes the simulation.

All successfully created threads are joined before their associated resources are released.
