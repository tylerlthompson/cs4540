# Description
This program is intended to simulate a process scheduler that uses a Round Robin priority queue.
Highest priority process gets run first. Process priority increases as it waits in the queue (aging) to prevent starvation.
A time quantum will kick a process back to the queue if it has been running too long.


# Compilation
To compile the program, run the following from the root directory of the project. `make source`

The project can also be cleaned with `make clean`

# Execution

The program takes one argument on the command line:
* file - The path to the file to read. Formatted `<priority> <cpu> <io> <runtime>`

Here is an example: `./source ./a2in.txt`

Optionally, make can also be used: `make run`
