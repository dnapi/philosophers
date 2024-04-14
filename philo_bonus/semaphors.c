#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <fcntl.h>

#define NUM_CHILDREN 10
#define MAX_SEM_COUNT 10

int main() {
    // Initialize semaphore with a maximum count of 10
    sem_t *sem = sem_open("/my_semaphore", O_CREAT, 0644, MAX_SEM_COUNT);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Fork multiple child processes
    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();

        if (pid < 0) {
            perror("fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Child process
            printf("Child %d started\n", i);
            // Child process waits on the semaphore
            if (sem_wait(sem) < 0) {
                perror("sem_wait");
                exit(EXIT_FAILURE);
            }
            printf("Child %d in critical section\n", i);
            // Simulating some work in the critical section
            usleep(1000000); // Sleep for 1 second
            // Child process releases the semaphore
            if (sem_post(sem) < 0) {
                perror("sem_post");
                exit(EXIT_FAILURE);
            }
            printf("Child %d completed\n", i);
            exit(EXIT_SUCCESS);
        }
    }

    // Parent process waits for all child processes to finish
    for (int i = 0; i < NUM_CHILDREN; i++) {
        if (wait(NULL) < 0) {
            perror("wait");
            exit(EXIT_FAILURE);
        }
    }

    // Close and unlink the semaphore
    if (sem_close(sem) < 0) {
        perror("sem_close");
        exit(EXIT_FAILURE);
    }
    if (sem_unlink("/my_semaphore") < 0) {
        perror("sem_unlink");
        exit(EXIT_FAILURE);
    }

    return 0;
}

