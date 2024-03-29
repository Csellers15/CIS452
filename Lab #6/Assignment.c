#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/sem.h>

#define SIZE 16

int main (int argc, char **argv)
{
    int status;
    struct sembuf sbuf;
    long int i, loop, temp, *shmPtr;
    int shmId;
    pid_t pid;
    int semId;
    int sem_ctl_result;

    if ((semId = semget(IPC_PRIVATE, 1, 00600)) < 0) {
        printf("Error fetching semaphore id\n");
        exit(1);
    }

    if ((sem_ctl_result = semctl(semId, 0, SETVAL, 1)) < 0) {
        printf("Error initlaizing semaphore\n");
        exit(1);
    }

    // Setup inital state of sbuf according to man pages 
    sbuf.sem_num = 0;
    sbuf.sem_op = 1;
    sbuf.sem_flg = 0;

    loop = atoi(argv[1]); 

    if ((shmId = shmget (IPC_PRIVATE, SIZE, IPC_CREAT | S_IRUSR | S_IWUSR)) < 0) {
        perror ("Error retrieving shared memory id\n");
        exit (1);
    }
    if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
        perror ("Error attaching to shared memory\n");
        exit (1);
    }

    shmPtr[0] = 0;
    shmPtr[1] = 1;

    if (!(pid = fork())) {
        for (i=0; i<loop; i++) {

            // Configure sbuf to decrement sem val
            sbuf.sem_op = -1;
            sbuf.sem_flg = SEM_UNDO;
            semop(semId, &sbuf, 1);


            temp = shmPtr[1];
            shmPtr[1] = shmPtr[0];
            shmPtr[0] = temp;

            // Configure sbuf to increment sem val
            sbuf.sem_op = 1;
            semop(semId, &sbuf, 1);
        }

        if (shmdt (shmPtr) < 0) {
            perror ("Error detaching from shared memory\n");
            exit (1);
        }
        exit(0);
    } else {
        for (i=0; i<loop; i++) {
            // Configure sbuf to decrement sem val
            sbuf.sem_op = -1;
            sbuf.sem_flg = SEM_UNDO;
            semop(semId, &sbuf, 1);

            // swap the contents of shmPtr[1] and shmPtr[0]
            temp = shmPtr[0];
            shmPtr[0] = shmPtr[1];
            shmPtr[1] = temp;

            // Configure sbuf to increment sem val
            sbuf.sem_op = 1;
            semop(semId, &sbuf, 1);
        }
    }

    wait (&status);

    // Clean up semaphore after it is no longer needed
    if ((sem_ctl_result = semctl(semId, 0, IPC_RMID)) < 0) {
        printf("Error removing semaphore\n");
        exit(1);
    }

    printf ("values: %li\t%li\n", shmPtr[0], shmPtr[1]);

    if (shmdt (shmPtr) < 0) {
        perror ("Error detaching from shared memory\n");
        exit (1);
    }
    
    if (shmctl (shmId, IPC_RMID, 0) < 0) {
        perror ("Error deallocating shared memory\n");
        exit(1);
    }

    return 0;
}