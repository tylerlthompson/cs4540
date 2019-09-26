/**
* Tyler Thompson
* WMU CS4540 - A2
* Oct 1, 2019
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "a2.h"

#define PROCESS_COUNT 48
#define QUANTUM 70

int open_file(FILE * * in_file, char * file_path) {
    int retval = 1;
    if (!(*in_file = fopen(file_path, "r"))) { 
        printf("Failed to open file >%s< Check that your path is correct.\n", file_path);
        retval = 0;
    }
    return retval;
}

void read_file(FILE * in_file, process * a) {
    int i;
    ui priority, cpu, io, runtime;
    for (i=0; fscanf(in_file, " %u %u %u %u", &priority, &cpu, &io, &runtime) != EOF && i<PROCESS_COUNT; i++) {
        // printf("Index: %d Priority: %u CPU: %u IO: %u Runtime: %u\n", i, priority, cpu, io, runtime);
        a[i].priority = priority;
        a[i].cpu = cpu;
        a[i].io = io;
    }
}

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

void add_process_to_queue(process proc, process * procs, ui * queue, ui * queue_count) {
    int i;
    for (i=queue_count-1; i >= 0 && procs[i].priority > proc.priority; i--) {
        procs[i+1] = procs[i];
    }
    procs[i+1] = proc;
}

int main(int argc, char * argv[]) {
    process a[PROCESS_COUNT]; 
    ui queue[PROCESS_COUNT]; 
    ui queue_count = 0;
    ui io[PROCESS_COUNT];  
    ui io_count = 0;
    ui cpu;
    os my_os = { .quantum=QUANTUM, .wait=30 };

    FILE * in_file = NULL;
    char * file_path;
    if (get_file_path(argc, argv, &file_path)) {
        if (open_file(&in_file, file_path)) {
            read_file(in_file, a);
            
        }
    } 

    printf("Done.\n");
    return 0;
}
