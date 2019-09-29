/**
* Tyler Thompson
* WMU CS4540 - A2
* Oct 1, 2019
*/
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "file_util.h"

/**
 * Function prototypes
 */
void load_cpu_proc(process * procs, ui * cpu, ui new_cpu, ui * queue_count);
void add_proc_to_queue(process * procs, ui * queue, ui * queue_count, int index);
void add_all_procs_to_queue(process * proces, ui * queue, ui * queue_count, const int process_count);
void swap(ui *x, ui *y);
void sort_queue(process * procs, ui * queue, ui queue_count);
void update_wait_min_max(process * procs, int index);
void age_ready_queue(process * procs, ui * queue, ui queue_count, ui wait);
void process_io_queue(process * procs, ui * io, ui * io_count, ui * queue, ui * queue_count);
void print_proc(process proc);
void print_procs(process * procs, const int process_count);
void print_queue(ui * queue, process * procs, const int process_count);


int main(int argc, char * argv[]) {
    const int process_count = 48;
    const int cycle_count = 10000;

    os my_os = { .quantum=70, .wait=30 };

    process a[process_count]; 
    ui queue[process_count]; 
    ui io[process_count]; 

    ui queue_count = 0, io_count = 0, cpu, cycle, last_cpu;

    FILE * in_file = NULL;
    char * file_path;

    if (get_file_path(argc, argv, &file_path)) {
        if (open_file(&in_file, file_path)) {

            // prepare array of processes from file
            read_file(in_file, a, process_count);
            add_all_procs_to_queue(a, queue, &queue_count, process_count);

            // load first process into cpu
            load_cpu_proc(a, &cpu, queue[queue_count-1], &queue_count);

            // run the cpu
            for (cycle=0; cycle<cycle_count; cycle++) {

                /**
                 * check if the running process is done 
                 * or has hit the cpu quantum
                 * move process to IO queue if done
                 * move process back to ready queue if quantum reached
                 */
                if (a[cpu].curCpu == a[cpu].cpu) { // process is done. move to IO
                    a[cpu].curCpu = 0;

                    io[io_count] = cpu; // move process to IO queue
                    io_count++;
                    
                    load_cpu_proc(a, &cpu, queue[queue_count-1], &queue_count); // load next process from ready queue
                }
                else if (a[cpu].curCpu == my_os.quantum) { // process hit quantum limit. move back to ready/wait queue
                    last_cpu = cpu;
                    load_cpu_proc(a, &cpu, queue[queue_count-1], &queue_count); // load next process from ready queue
                    add_proc_to_queue(a, queue, &queue_count, last_cpu); // move process back to wait queue
 
                }

                /**
                 * while keeping the ready queue sorted by curPrior, 
                 * increase the processes curPrior based on wait time in queue
                 */
                age_ready_queue(a, queue, queue_count, my_os.wait); 

                /**
                 * Increment process curIo and move processes back to ready queue when
                 * they are done
                 */
                process_io_queue(a, io, &io_count, queue, &queue_count);
                
                a[cpu].curCpu++;
                a[cpu].cpuTotal++;
            }
            printf("CPU Cycle Count: %d Process Count: %d\n", cycle_count, process_count);
            print_procs(a, process_count);
            printStats(a, my_os);
        }
    } 
    return 0;
}

/**
 * Load a new process into the cpu
 */
void load_cpu_proc(process * procs, ui * cpu, ui new_cpu, ui * queue_count) {
    *cpu = new_cpu; // load next process
    update_wait_min_max(procs, *cpu);
    procs[*cpu].wait = 0;
    procs[*cpu].curPrior = procs[*cpu].priority;
    (*queue_count)--;
}

/**
 * Add a process to the ready queue using insert sort
 * Ready queue is always sorted by curPrior
 */
void add_proc_to_queue(process * procs, ui * queue, ui * queue_count, int index) {
    int i;
    procs[index].curPrior = procs[index].priority;
    for (i=*queue_count-1; i >= 0 && procs[queue[i]].curPrior > procs[index].curPrior; i--) {
        queue[i+1] = queue[i];
    }
    queue[i+1] = index;
    (*queue_count)++;
    procs[index].waitCount++;
    procs[index].wait = 0;
}

/**
 * Add an array of processes to the ready queue
 */
void add_all_procs_to_queue(process * proces, ui * queue, ui * queue_count, const int process_count) {
    int i;
    for (i=0; i<process_count; i++) {
        add_proc_to_queue(proces, queue, queue_count, i);
    }

}

/**
 * Swaps the data at two memory locations of type ui
 */
void swap(ui *x, ui *y) {  
    ui temp = *x;  
    *x = *y;  
    *y = temp;  
}

/**
 * Sort the ready queue based on curPrior using bubble sort
 */
void sort_queue(process * procs, ui * queue, ui queue_count) {
    int i, j;
    for (i=0; i<queue_count-1; i++){
        for (j=0; j<queue_count-i-1; j++) {
            if (procs[queue[j]].curPrior > procs[queue[j+1]].curPrior) {
                swap(&queue[j], &queue[j+1]);
            }
        }
    }
}

/**
 * Update the waitMin and waitMax attributes of a process
 */
void update_wait_min_max(process * procs, int index) {
    if (procs[index].waitMax == 0 || procs[index].wait > procs[index].waitMax) procs[index].waitMax = procs[index].wait;
    if (procs[index].waitMin == 0 || procs[index].wait < procs[index].waitMin) procs[index].waitMin = procs[index].wait;
}

/**
 * Scan the ready queue and increase wait and curPrior as needed
 * Ready queue will be left sorted by curPrior
 */
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

/**
 * Cheak the processes in the IO queue and move them out if they are done
 */
void process_io_queue(process * procs, ui * io, ui * io_count, ui * queue, ui * queue_count) {
    int i;
    for (i=0; i<*io_count; i++) {
        if (procs[io[i]].curIo == procs[io[i]].io) { // process ready to leave IO
            procs[io[i]].curIo = 0; // reset curIo
            add_proc_to_queue(procs, queue, queue_count, io[i]); // move process to ready queue

            // shift the IO queue
            int j;
            for (j=i; j<*io_count; j++) {
                io[j] = io[j+1];
            }
            (*io_count)--;
            i--;
        }
        else { // increment IO 
            procs[io[i]].curIo++;
            procs[io[i]].ioTotal++;
        }
    }
}

/**
 * Print all the attributes of a process
 */
void print_proc(process proc) {
//    printf("Priority: %-2u CPU: %u IO: %u curCPU: %-2u curIo %-4u wait: %-4u curPrior: %-4u cpuTotal: %-4u ioTotal: %-4u waitSum: %-6u waitCount: %-4u waitMin: %-4u waitMax: %-4u\n",
//        proc.priority, proc.cpu, proc.io, proc.curCpu, proc.curIo, proc.wait, proc.curPrior, proc.cpuTotal, proc.ioTotal, proc.waitSum, proc.waitCount, proc.waitMin, proc.waitMax);
    printf("%-8u %-3u %-2u %-6u %-5u %-4u %-8u %-8u %-7u %-7u %-9u %-7u %-7u\n", proc.priority, proc.cpu, proc.io, proc.curCpu, proc.curIo, proc.wait, proc.curPrior, proc.cpuTotal, proc.ioTotal, proc.waitSum, proc.waitCount, proc.waitMin, proc.waitMax);
}


/**
 * Print all the attributes of an array of processes
 */
void print_procs(process * procs, const int process_count) {
    int i;
    printf("Priority CPU IO curCPU curIo wait curPrior cpuTotal ioTotal waitSum waitCount waitMin waitMax\n");
    for (i=0; i<process_count; i++) {
        print_proc(procs[i])    ;
    }
}

/**
 * Print a summary of the processes in a queue
 */
void print_queue(ui * queue, process * procs, const int process_count) {
    int i;
    for (i=0; i<process_count; i++) {
        printf("%d Index: %d Priority: %u curPrior: %u\n", i, queue[i], procs[queue[i]].priority, procs[queue[i]].curPrior);
    }
}
