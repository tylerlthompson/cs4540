/**
* Tyler Thompson
* WMU CS4540 - A3
* Oct 8, 2019
*/
#include <stdio.h>  
#include <string.h>
#include <stdlib.h> 
#include <fcntl.h>
#include <semaphore.h>
#include <time.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define NUM_PROCESSES 7

void Process(sem_t * screen, sem_t * keyboard, int index);
int getSemaphores(sem_t * screen, sem_t * keyboard, int index);
void handle_sigterm();


int main(int argc, char * argv[]) {
    sem_t * sems, * semk;
    int pids[NUM_PROCESSES]; 
    int pid, i = 0, j = 0;

    // sem_open both semaphores
    sems = sem_open("/sems", O_CREAT, 0600, 1);
    semk = sem_open("/semk", O_CREAT, 0600, 1);

    do {
        if( (pid = fork()) == 0) {  // fork 7 processes
            Process(sems, semk, i+1);
        }
        else {
            pids[i] = pid;
            i++;
        }
    } while( i < NUM_PROCESSES && pid > 0);

    if(i >= 7) { // stop forking around
        printf("Running semaphore assignment using processes\n");
        signal(SIGTERM, handle_sigterm);
        signal(SIGINT, handle_sigterm);
        // after all die, use single call or loop based on ids saved above
        for(j=0; j<NUM_PROCESSES; j++) waitpid(pids[j], NULL, 0);

        // sem_unlink both
        sem_unlink("/sems");
        sem_unlink("/semk");

        printf("Done.\n");
    }
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
 * The main function that is run in each process
 */
void Process(sem_t * screen, sem_t * keyboard, int index) {
    int count = 0, status, id = getpid();
    char input[80];

    // sem_open both semaphores if necessary
    if (sem_getvalue(keyboard, &status) == -1) keyboard = sem_open("/semk", O_CREAT, 0600, 0);
    if (sem_getvalue(screen, &status) == -1) screen = sem_open("/sems", O_CREAT, 0600, 0);
    
    do { //	loop
        count += getSemaphores(screen, keyboard, index);

        // prompt id + " enter < 80 characters " + id + " or q to quit: "
        printf("%d enter < 80 characters %d or q to quit\n", id, id);
        fscanf(stdin, " %s", input);

        // echo what was typed with id before and after echo
        // echo id + " start " + what was typed + id + " end"
        printf("%d start >%s< %d end\n", id, input, id);

        // sem_post or give back both semaphores
        sem_post(screen);
        sem_post(keyboard);

    } while (strcmp(input, "q") != 0); // until quit end inner loop in C do ... while not q

    count += getSemaphores(screen, keyboard, index);

    // prompt "This process " + id + " had " + count + " deadlocks "
    printf("This process %d had %d deadlocks\n", id, count);

    // sem_post or give back both semaphores
    sem_post(screen);
    sem_post(keyboard);

    // sem_close 
    sem_close(screen);
    sem_close(keyboard);

    // exit
    exit(0);
}

// function to get semaphores (screen, keyboard, index)
int getSemaphores(sem_t * screen, sem_t * keyboard, int index) {
    // returns count of how many times recovered from deadlock
    int count = 0, have_both = 0;
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
        if (sem_timedwait(*second_sem, &ts) != 0) { // if timeout 
            
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
    } while(have_both == 0); // until have both
    return count;
} // end get semaphores function
