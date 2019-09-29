/**
* Tyler Thompson
* WMU CS4540 - A2
* Oct 1, 2019
*/

#ifndef FILE_UTIL_H
#define FILE_UTIL_H
#include <stdio.h>
#include "a2.h"

/**
 * Function Prototypes
 */
int open_file(FILE * * in_file, char * file_path);
void read_file(FILE * in_file, process * a, const int process_count);
int get_file_path(int argc, char * argv[], char * * file_path);

/**
 * Attempt to open a file
 */
int open_file(FILE * * in_file, char * file_path) {
    int retval = 1;
    if (!(*in_file = fopen(file_path, "r"))) { 
        printf("Failed to open file >%s< Check that your path is correct.\n", file_path);
        retval = 0;
    }
    return retval;
}

/**
 * Read a file into an array of processes
 */
void read_file(FILE * in_file, process * a, const int process_count) {
    int i;
    ui priority, cpu, io, runtime;
    for (i=0; fscanf(in_file, " %u %u %u %u", &priority, &cpu, &io, &runtime) != EOF && i<process_count; i++) {
        a[i].priority = priority;
        a[i].cpu = cpu;
        a[i].io = io;
        a[i].curCpu = 0;
        a[i].curIo = 0;
        a[i].wait = 0;
        a[i].curPrior = priority;
        a[i].cpuTotal = 0;
        a[i].ioTotal = 0;
        a[i].waitSum = 0;
        a[i].waitCount = 0;
        a[i].waitMin = 0;
        a[i].waitMax = 0;
    }
}

/**
 * Parse a file path from command line arguments
 */
int get_file_path(int argc, char * argv[], char * * file_path) {
    int retval = 1;
	if (argc != 2) {
		printf("Incorrect number of arguments pass. Usage: %s [filename]\n", argv[0]);
		retval = 0;
	}
	else { 
		*file_path = argv[1];
	}
	return retval;
}

#endif