
#include <stdio.h>  
#include <string.h>
#include <stdlib.h> 
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>

void Process(sem_t * screen, sem_t * keyboard, int index);
int getSemaphores(sem_t * screen, sem_t * keyboard, int index);

int main(int argc, char * argv[]) {

//   sem_open both semaphores

//   fork 7 processes 
// 	Each process passed index of loop so 1,2,3, or 4
//   Below is real C to show just how easy this is, please do not overthink

    sem_t * sems, * semk;
    sems = sem_open("/sems", O_CREAT, 0644, 1);
    semk = sem_open("/semk", O_CREAT, 0644, 1);

    int pids[7]; int pid;
    int i = 0;
    int child_in[2];
    pipe(child_in);
    do {
        
        if( (pid = fork()) == 0) {
            
            dup2(child_in[0], 0);

            close(child_in[1]);
            Process(sems, semk, i+1);
        }
        else {
            pids[i] = pid;
            i++;
            int status;
            waitpid(pid, &status, WNOHANG);
            dup2(child_in[1], 1);
            close(child_in[0]);
        }
        //printf("Pid: %d Index: %d\n", pid, i);
    } while( i < 7 && pid > 0);

    if(i >= 7) { // stop forking around
        // after all die, use single call or loop based on ids saved above
        // sem_unlink both
        
        sem_unlink("/sems");
        sem_unlink("/semk");

        // dup2(child_in[0], 0);
        // close(child_in[0]);
        // close(child_in[1]);

        printf("Done.\n");
    }
    return 0;
}   


void Process (sem_t * screen, sem_t * keyboard, int index) {
    // sem_open both semaphores if necessary
    //screen = sem_open("/sems", O_CREAT, 0644, 0);
    //keyboard = sem_open("/semk", O_CREAT, 0644, 0);

    // count = 0
    int count = 0;
    char input[80];
 	int id = getpid();
    //printf("Processing %d\n", id);
    
    do { //	loop
        //printf("Pid: %d Count: %d\n", id, count);
        // count += getSemaphores(index)
        count += getSemaphores(screen, keyboard, index);
        printf("Pid: %d Count: %d\n", id, count);

        // prompt id + " enter < 80 characters " + id + " or q to quit: "
        printf("%d enter < 80 characters %d or q to quit\n", id, id);
        //sleep(10);
        // read keboard
        // if (fgets(input, 80, stdin) == 0) {
        //     printf("Failed to read input on pid %d", id);
        //     exit(0);
        // }
        getc(stdin);
        fscanf(stdin, " %s", input);

        //printf("StRCMP: %d\n", strcmp(input, "q"));
        // echo what was typed with id before and after echo
        // echo id + " start " + what was typed + id + " end"
        printf("%d start >%s< %d end\n", id, input, id);

        // sem_post or give back both semaphores
        sem_post(screen);
        sem_post(keyboard);

    } while (strcmp(input, "q") != 0); // until quit end inner loop in C do ... while not q

    // count += getSemaphores(index)
    count += getSemaphores(screen, keyboard, index);

    // prompt "This process " + id + " had " + count + " deadlocks "
    printf("This process %d has %d deadlocks", id, count);

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
    // count = 0
    int count = 0;
    int have_both = 0;
    int semk_value = 0;
    int sems_value = 0;
    int timed_wait = 0;

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
        sleep(1);

        // sem_timedwait with timer for second
        ts.tv_sec = 3;
        ts.tv_nsec = 0;
        timed_wait = sem_timedwait(*second_sem, &ts);
        if (timed_wait != 0) { // if timeout 
            
            // give back first, (sem_post)
            sem_post(*first_sem);
            // wait random time
            sleep(rand() % 3);
            // increment count
            count++;
        }
        else { // else
            // have both  true   
            have_both = 1;
            printf("Now we have both\n");
        }

        sem_getvalue(keyboard, &semk_value);
        sem_getvalue(screen, &sems_value);
        //printf("Pid: %d Have Both: %d Count: %d Index: %d Semk: %d Sems: %d TimedWait: %d ErrorNo %d\n", getpid(), have_both, count, index, semk_value, sems_value, timed_wait, errno);
    } while(have_both == 0); // until have both

//  return count
    return count;
// end get semaphores function
}
