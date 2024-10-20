#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <time.h>

// Shared memory and semaphore variables
char *buffer;
sem_t *mutex, *empty, *full;
int *next;

void producer(int id) {
    
    srand(time(NULL) + id);
    
    while (1) {
        // Sleep for random time [0, 4] seconds
        sleep(rand() % 5);

        // Produce a random character
        char c = 'a' + rand() % 26;

        sem_wait(empty);
        sem_wait(mutex);

        // Add the produced character to the buffer
        buffer[*next] = c;
        printf("-- [P%d] Produced [%d,%c]\n", id, *next, c);
        // Increment "next" circularly
        *next = (*next + 1) % 100;

        sem_post(mutex);
        sem_post(full);   
    }
}

void consumer(int id) {
    srand(time(NULL) + id);
    while (1) {
        // Sleep for random time [0, 4] seconds
        sleep(rand() % 5);

        sem_wait(full);
        sem_wait(mutex);

        // Remove a character from the buffer
        int index = (*next - 1 + 100) % 100;
        char c = buffer[index];
        printf("----- [C%d] Consumed [%d,%c]\n", id, index, c);
        // Decrement "next" circularly
        *next = (*next - 1 + 100) % 100;

        sem_post(mutex);
        sem_post(empty);
    };
};

int main(int argc, char *argv[]) {
  

    // # producer, #consumer, size of buffer
    if (argc != 4) {
        printf("Usage: %s P C N\n", argv[0]);
        exit(EXIT_FAILURE);
    };

    // to ints
    int P = atoi(argv[1]);
    int C = atoi(argv[2]);
    int N = atoi(argv[3]);



    // Shared memory for buffer
    // IPC_PRIVATE temporary and no key
    int shmid_buf = shmget(IPC_PRIVATE, sizeof(char) * N, IPC_CREAT | 0666); 
    buffer = (char*)shmat(shmid_buf, NULL, 0); 

    
    // Shared memory for next
    int shmid_next = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666);
    next = (int *)shmat(shmid_next, NULL, 0);

    // Initialize buffer and next
    *next = 0;

    // Initialize semaphores
    mutex = sem_open("/mutex", O_CREAT | O_EXCL, 0666, 1);
    empty = sem_open("/empty", O_CREAT | O_EXCL, 0666, N);
    full =  sem_open("/full",  O_CREAT | O_EXCL, 0666, 0);

    // Fork producers
    for (int i = 0; i < P; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            producer(i + 1);
            exit(EXIT_SUCCESS);
        }
    }

    // Fork consumers
    for (int i = 0; i < C; i++) {
        pid_t pid = fork();
        if (pid == 0) {
            consumer(i + 1);
            exit(EXIT_SUCCESS);
        }
    }




    // Wait for all child processes to finish
    for (int i = 0; i < P + C; i++) {
        wait(NULL);
    }

    // Clean up shared memory and semaphores
    shmctl(shmid_buf, IPC_RMID, NULL);
    shmctl(shmid_next, IPC_RMID, NULL);
    sem_unlink("/mutex");
    sem_unlink("/empty");
    sem_unlink("/full");

    return 0;
}
