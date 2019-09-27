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
    int retval = 1;
    if (!(*in_file = fopen(file_path, "r"))) { 
        printf("Failed to open file >%s< Check that your path is correct.\n", file_path);
        retval = 0;
    }
    return retval;
}

void read_file(FILE * in_file, process * a, const int process_count) {
    int i;
    ui priority, cpu, io, runtime;
    for (i=0; fscanf(in_file, " %u %u %u %u", &priority, &cpu, &io, &runtime) != EOF && i<process_count; i++) {
        // printf("Index: %d Priority: %u CPU: %u IO: %u Runtime: %u\n", i, priority, cpu, io, runtime);
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

void add_proc_to_queue(process * procs, ui * queue, ui * queue_count, int index) {
    int i;
    for (i=*queue_count-1; i >= 0 && procs[queue[i]].curPrior > procs[index].curPrior; i--) {
        queue[i+1] = queue[i];
    }
    queue[i+1] = index;
    (*queue_count)++;
    procs[index].waitCount++;
    procs[index].curPrior = 0;
    procs[index].wait = 0;
}

void add_all_procs_to_queue(process * proces, ui * queue, ui * queue_count, const int process_count) {
    int i;
    for (i=0; i<process_count; i++) {
        add_proc_to_queue(proces, queue, queue_count, *queue_count);
    }

}

void sort_queue(process * procs, ui * queue, ui queue_count) {
    int i, j;
    for (i=0; i<queue_count-1; i++){
        for (j=0; j<queue_count-i-1; j++) {
            if (procs[queue[j]].curPrior > procs[queue[j+1]].curPrior) {
                ui temp = queue[j];
                queue[j] = queue[j+1];
                queue[j+1] = temp;
            }
        }
    }
}

void age_ready_queue(process * procs, ui * queue, ui queue_count, ui wait) {
    int i;
    for (i=0; i<queue_count; i++) {
        if ((procs[queue[i]].wait % wait) == 0) {
            procs[queue[i]].curPrior++;
        }
        procs[queue[i]].wait++;
        procs[queue[i]].waitSum++;
    }
    sort_queue(procs, queue, queue_count);
}

void process_io_queue(process * procs, ui * io, ui * io_count, ui * queue, ui * queue_count) {
    int i;
    for (i=0; i<*io_count; i++) {
        if (procs[io[i]].curIo == procs[io[i]].io) {
            procs[io[i]].ioTotal += procs[io[i]].curIo;
            procs[io[i]].curIo = 0;

            add_proc_to_queue(procs, queue, queue_count, io[i]);

            // shift the queue
            int j;
            for (j=0; j<*io_count-i-1; j++) {
                io[j] = io[j+1];
            }
            (*io_count)--;
        }
        else {
            procs[io[i]].curIo++;
        }
    }
}

void print_proc(process proc) {
    printf("Priority: %-2u CPU: %u IO: %u curCPU: %-2u curIo %-4u wait: %-4u curPrior: %-4u cpuTotal: %-4u ioTotal: %-4u waitSum: %-6u waitCount: %-4u waitMin: %-4u waitMax: %-4u\n",
        proc.priority, proc.cpu, proc.io, proc.curCpu, proc.curIo, proc.wait, proc.curPrior, proc.cpuTotal, proc.ioTotal, proc.waitSum, proc.waitCount, proc.waitMin, proc.waitMax);
}

void print_procs(process * procs, const int process_count) {
    int i;
    for (i=0; i<process_count; i++) {
        print_proc(procs[i])    ;
    }
}

void print_queue(ui * queue, process * procs, const int process_count) {
    int i;
    for (i=0; i<process_count; i++) {
        printf("Index: %d Priority: %u curPrior: %u\n", i, procs[queue[i]].priority, procs[queue[i]].curPrior);
    }
}

int main(int argc, char * argv[]) {
    const int process_count = 48;
    process a[process_count]; 
    ui queue[process_count]; 
    ui queue_count = 0;
    ui io[process_count];  
    ui io_count = 0;
    ui cpu;
    os my_os = { .quantum=70, .wait=30 };
    ui cycle;
    ui next_cpu;

    FILE * in_file = NULL;
    char * file_path;
    if (get_file_path(argc, argv, &file_path)) {
        if (open_file(&in_file, file_path)) {
            read_file(in_file, a, process_count);
            add_all_procs_to_queue(a, queue, &queue_count, process_count);
            // print_procs(a, process_count);
            // print_queue(queue, a, process_count);
            printf("Queue Count: %u\n", queue_count);

            // load first process into cpu
            cpu = queue[queue_count];
            queue_count--;

            for (cycle=0; cycle<10000; cycle++) {
                //printf("Cycle: %u Processing: %u Queue Size: %u IO Size: %u\n", cycle, cpu, queue_count, io_count);
                //print_proc(a[cpu]);
                if (a[cpu].curCpu == a[cpu].cpu) {
                    io[io_count] = cpu; // move to IO queue
                    io_count++; 
                    a[cpu].cpuTotal += a[cpu].curCpu; // adjust cpu total
                    a[cpu].curCpu = 0;
                    cpu = queue[queue_count-1]; // load next process
                    queue_count--;
                    a[cpu].wait = 0;
                }
                else if (a[cpu].curCpu == my_os.quantum) {
                    next_cpu = queue[queue_count-1];
                    add_proc_to_queue(a, queue, &queue_count, queue_count); // move process back to wait queue
                    a[cpu].cpuTotal += a[cpu].curCpu; // adjust cpu total
                    a[cpu].curCpu = 0;
                    cpu = next_cpu; // load next process
                    queue_count--;
                    a[cpu].wait = 0;
                }

                age_ready_queue(a, queue, queue_count, my_os.wait);
                
                process_io_queue(a, io, &io_count, queue, &queue_count);

                a[cpu].curCpu++;

            }
        }
    } 
    print_procs(a, process_count);
    print_queue(queue, a, process_count);
    printStats(a, my_os);
    printf("Done.\n");
    return 0;
}
