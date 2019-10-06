/**
* Tyler Thompson
* WMU CS4540 - A3
* Oct 8, 2019
*/
#include <stdio.h>  
#include <string.h>
#include <stdlib.h> 
#include <semaphore.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

#define NUM_THREADS 7

void * Process(void * args_t);
int getSemaphores(sem_t * screen, sem_t * keyboard, int index);
void handle_sigterm();

/* arguments for a thread */
struct arg_struct {
    sem_t * screen;
    sem_t * keyboard;
    int index;
    int thread_complete;
};

int main(int argc, char * argv[]) {
    sem_t sems, semk;
    pthread_t threads[NUM_THREADS];
    struct arg_struct thread_args[NUM_THREADS];
    int i, done_threads = 0;

    printf("Running semaphore assignment using threads\n");

    // sem_open both semaphores
    sem_init(&sems, 0, 1);
    sem_init(&semk, 0, 1);

    // register signal hanglers
    signal(SIGTERM, handle_sigterm);
    signal(SIGINT, handle_sigterm);

    // create all threads
    for (i=0; i<NUM_THREADS; i++) {
        thread_args[i].keyboard = &semk;
        thread_args[i].screen = &sems;
        thread_args[i].index = i;
        thread_args[i].thread_complete = 0;
        pthread_create(&(threads[i]), NULL, &Process, (void *) &(thread_args[i]));
    }

    // wait until all threads are done
    while(done_threads != NUM_THREADS) {
        done_threads = 0;
        for (i=0; i<NUM_THREADS; i++) done_threads += thread_args[i].thread_complete;
        sleep(1);
    }

    // join all threads
    for (i=0; i<NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Done.\n");
    return 0;
}   

/**
 * Handle term signal by unlinking semaphores
 */
void handle_sigterm() {
    sem_unlink("/sems");
    sem_unlink("/semk");
    printf("Unlinking semaphores and terminating Pid: %d\n", getpid());
    exit(0);
}

/**
 * The main function that is run in each thread
 */
void * Process(void * args_t) {
    int status, count = 0;
    char input[80];
    struct arg_struct * args = args_t;

    // sem_open both semaphores if necessary
    if (sem_getvalue(args->keyboard, &status) == -1) sem_init(args->keyboard, 0, 1);
    if (sem_getvalue(args->screen, &status) == -1) sem_init(args->screen, 0, 1);
    
    do { //	loop
        count += getSemaphores(args->screen, args->keyboard, args->index);

        // prompt id + " enter < 80 characters " + id + " or q to quit: "
        printf("%d enter < 80 characters %d or q to quit\n", args->index, args->index);
        fscanf(stdin, " %s", input);

        // echo what was typed with id before and after echo
        // echo id + " start " + what was typed + id + " end"
        printf("%d start >%s< %d end\n", args->index, input, args->index);

        // sem_post or give back both semaphores
        sem_post(args->screen);
        sem_post(args->keyboard);

    } while (strcmp(input, "q") != 0); // until quit end inner loop in C do ... while not q

    count += getSemaphores(args->screen, args->keyboard, args->index);

    // prompt "This process " + id + " had " + count + " deadlocks "
    printf("This thread %d had %d deadlocks\n", args->index, count);

    // sem_post or give back both semaphores
    sem_post(args->screen);
    sem_post(args->keyboard);

    // sem_close 
    sem_close(args->screen);
    sem_close(args->keyboard);

    // exit
    args->thread_complete = 1;
    pthread_exit(0);
}

// function to get semaphores (screen, keyboard, index)
int getSemaphores(sem_t * screen, sem_t * keyboard, int index) {
    // returns count of how many times recovered from deadlock

    int count = 0, have_both = 0, semk_value = 0, sems_value = 0, timed_wait = 0;
    struct timespec ts;

    // can use temp and swap semphores
    sem_t * * first_sem;
    sem_t * * second_sem;
    if (index % 2 == 0) { // odd index gets screen first
        first_sem = &screen;
        second_sem = &keyboard;
    } 
    else { // even index gets keyboard first
        first_sem = &keyboard;
        second_sem = &screen;
    }

    do { // loop 
        
        // sem_wait for first
        sem_wait(*first_sem);

        // sleep(1) // only use this if no deadlocks without it should not be necessary
        if (count == 0 ) sleep(1);

        // sem_timedwait with timer for second
        ts.tv_sec = 0;
        ts.tv_nsec = 10000;
        timed_wait = sem_timedwait(*second_sem, &ts);
        if (timed_wait != 0) { // if timeout 
            
            // give back first, (sem_post)
            sem_post(*first_sem);
            // wait random time
            usleep((rand() % 3) * 1000000);
            // increment count
            count++;
        }
        else { // else
            // have both  true   
            have_both = 1;
        }

        sem_getvalue(keyboard, &semk_value);
        sem_getvalue(screen, &sems_value);
    } while(have_both == 0); // until have both

    return count;
} // end get semaphores function
