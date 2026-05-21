*This project has been created as part of the 42 curriculum by yghergho*

# CODEXION

## Description

**Context**  
One or more coders work in a circular working space with a shared compiler in the center. The coders need to compile, debug and refactor (they can't debug nor refactor while they debug, refactor nor compile while debug etc..)  
There are also USB dongles, as many as coders.
In order to compile coders need 2 dongles plugged at the same time, their left one and right one.  
Once compilation is done, dongles are put back and debug can start, then refactor. The simulation stops once the number of compilation required is reached or once a coder burns out (lack of compiling).  
Coders need to compile regularly to avoid burning out. Coders don't communicate with each other and don't know who's about to burn out.

**Technical requirements**  
The code is written in **C**, each coder is represented by a **thread** and dongles by a **mutex**.  
While all coder threads are executing their routines, a **monitor thread** will iterate over each coder throughout the simulation to check whether a coder has reached its deadline and declare its burnout or if each coder has reached the amount of compilation required to stop the simulation and display the right log.

**Technical Challenges**  
The purpose of this project is to handle concurrent programming and the complexities of multithreading. Because multiple coders (threads) run simultaneously and compete for limited shared resources (dongles), the core challenge is to manage their execution without crashing or freezing the system. The simulation must overcome concurrency issues:

- **Data Races**: the program must prevent multiple threads from reading or modifying the same variable (last compilation time or simulation status) at the exact same time.

- **Deadlocks**: the program must avoid situations where coders wait indefinitely for each other to release a dongle (classic circular wait problem), which would freeze the entire simulation.

- **Resource Starvation**: in case of ``edf`` scheduler, the program must ensure a fair distribution dongles so that no coder is left waiting for so long that they burn out.

## Instructions

#### Compilation
To compile the program, run the ``make`` command, then a ``codexion`` executable should be created.

To run the program, launch ``./codexion`` followed by all the required **parameters**:
- ``number_of_coders``
- ``time_to_burnout`` in milliseconds
- ``time_to_compile`` in milliseconds
- ``time_to_debug`` in milliseconds
- ``time_to_refactor`` in milliseconds
- ``number_of_compiles``
- ``dongle_cooldown`` in milliseconds
- ``scheduler`` "fifo" or "edf"

All parameters except scheduler must be numerical values superior to 0. Scheduler has to be in lower case.

Depending on your entry, the simulation end either with all coders completing the number of compilations required or with a coder burning out with the following format :  
`` Time since simulation start _ coder id _ action``

#### Compilation completion example:
```c
./codexion 2 600 60 60 60 2 30 edf
0 1 has taken a dongle
1 1 has taken a dongle
1 1 is compiling
61 2 has taken a dongle
61 2 has taken a dongle
61 1 is debuging
61 2 is compiling
121 1 is refactoring
121 2 is debuging
181 1 has taken a dongle
181 1 has taken a dongle
181 1 is compiling
181 2 is refactoring
241 1 is debuging
241 2 has taken a dongle
241 2 has taken a dongle
241 2 is compiling
301 1 is refactoring
301 2 is debuging
301 All coders have completed their compilations

```
#### Burn out example:
```c
./codexion 2 60 60 60 60 3 30 edf 
0 2 has taken a dongle
0 2 has taken a dongle
0 2 is compiling
60 1 burned out!
```
Each "``is compiling``" log is preceded by two "``has taken a dongle``" log and a "``burned out``" log should occurs 10 ms after the actual coder burn out.

#### Leaks tests
To test leaks or threads, mutex, condition and data race errors you can run the following commands:

```c
valgrind ./codexion 2 600 60 60 60 2 30 edf
valgrind --tool=drd ./codexion 2 600 60 60 60 2 30 edf
valgrind --tool=helgrind ./codexion 2 600 60 60 60 2 30 edf
```

#### Clean
``make clean`` will delete all ``.o`` files  
``make fclean`` will do the same as above + delete ``codexion`` executable
## Blocking cases handled
To ensure a flawless simulation without any freezes or unfair behavior, several concurrency issues were addressed:

- **Deadlock Prevention**: Coffman's circular wait condition is broken by ensuring coders never hold one dongle while waiting for another. The ``edf`` scheduler only attributes dongles when both the left and right ones are available simultaneously. Furthermore, strict lock ordering (always locking ``heap_lock`` before ``run_lock``) is respected throughout the code to prevent threads from waiting on each other indefinitely.

- **Starvation Prevention** (EDF Scheduler): In the Earliest Deadline First mode, starvation is prevented by sorting coders based on their remaining time before burnout. In case of a tie, the tie-breaker mechanism prioritizes the coder with the least amount of compiles, and then the one who requested the dongles first (ticket system).

- **Precise Burnout Detection**: The startup race condition is handled by initializing all coders' timers (``last_compile``) from the main thread before broadcasting the start signal. This ensures the monitor thread doesn't mistakenly declare a coder dead at the very first millisecond due to initialization delays.

- **Cooldown Handling**: A mandatory dongle cooldown is enforced after a compilation. This prevents a fast coder from immediately re-locking the same dongles and monopolizing the shared resources.

- **Log Serialization**: A dedicated ``print_lock`` ensures that ``printf`` calls are never interleaved. Additionally, the simulation state (``is_running``) is checked before and after taking the dongles to prevent any action from being printed after a burnout has been declared.

## Thread synchronization mechanisms
This project relies heavily on POSIX threading primitives to coordinate the coders and the monitor safely:

#### *``Pthread_mutex_t``*: 
Used extensively to protect shared memory and avoid Data Races.

- **``compile_lock``**: Protects each coder's ``last_compile`` time and ``number_of_compiles``. The monitor safely locks it to read the state while the coder locks it to update its values.

- **``run_lock``**: Protects the global ``is_running`` boolean, ensuring thread-safe checks before any major action (compiling, debugging, refactoring).

- **``heap_lock``**: Protects the priority queue (heap) used by the EDF scheduler to dispatch dongles.

#### ``Pthread_cond_t`` (Condition Variables): 
Used to prevent active polling (CPU burning) and handle precise wake-ups.

- **The Synchronized Start (``start_cond``)**: All threads wait on this condition upon creation. The main thread then broadcasts a single signal, making all coders and the monitor start at the exact same microsecond.

- **The Wait List (``heap_cond``)**: Instead of continuously checking for dongle availability, coders missing resources go to sleep (``pthread_cond_wait``). When a coder finishes compiling and unlocks their dongles, a ``pthread_cond_broadcast`` is sent to wake up the waiting coders so the dispatcher can allocate the newly freed resources. The monitor also uses this broadcast to wake up any sleeping coders when the simulation ends, preventing a final deadlock.

## Resources
- [Threads and mutex usage](https://www.codequoi.com/en/threads-mutexes-and-concurrent-programming-in-c/) 
- [Heap implementation](https://www.geeksforgeeks.org/c/c-program-to-implement-priority-queue/)
- [Pthread condition](https://www.ibm.com/docs/fr/aix/7.3.0?topic=p-pthread-cond-signal-pthread-cond-broadcast-subroutine)
#### AI Usage
- Concepts clarification
- Debugg
- Test
- Readme organization

