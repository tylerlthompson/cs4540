/**
* Tyler Thompson
* WMU CS4540 - A2
* Oct 1, 2019
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "a2.h"

int open_file(FILE * * in_file, char * file_path) {
    int retval = 0;
    if (!(*in_file = fopen(file_path, "r"))) { 
        printf("Failed to open file %s Check that your path is correct.\n", file_path);
        retval = 1;
    }
    return retval;
}

void read_file(FILE * in_file, process * * a) {
    int i;
    ui priority, cpu, io, runtime;
    for (i=0; fscanf(in_file, " %u %u %u %u", &priority, &cpu, &io, &runtime) != EOF && i<48; i++) {
        printf("Index: %d Priority: %u CPU: %u IO: %u Runtime: %u\n", i, priority, cpu, io, runtime);
        (*a[i]).priority = priority;
        (*a[i]).cpu = cpu;
        (*a[i]).io = io;
    }
}

int main(int argc, char * argv[]) {
    process a[48]; 
    // process id is array index
    // variables below hold index to process in this array (process id)
    ui queue[48]; 
    ui queueCount = 0;
    ui io[48];  
    ui ioCount = 0;
    ui cpu;

    a->priority = malloc(sizeof(ui));

    FILE * in_file = NULL;

    open_file(&in_file, "./a2in.txt");
    read_file(in_file, &a);  

    return 0;
}
