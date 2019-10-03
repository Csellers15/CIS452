#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define SIZE 4096

void sigHandler(int);

key_t key;
int shmId;
char *shmPtr;

typedef struct {
    int turn;
    char message[1024];
    int count;
} DataShared;

int main() {
    DataShared data;
    data.turn = 0;
    data.count = 0;
    signal(SIGINT, sigHandler);

    key = ftok("mkey",65);

    if((shmId = shmget(key, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0 ) {
        perror("Error creating shared memory\n");
        exit(1);
    }

    if((shmPtr = shmat(shmId, 0, 0)) == (void*) -1) {
        perror("Can't attach\n");
        exit(1);
    }

    while(1) {
        while (data.turn) {
            memcpy(&data, shmPtr, sizeof(DataShared));
        }
        
        // enter critical section
        printf("Enter a message: \n" );
        fgets(data.message, 1024, stdin);

        
        // leave critical section
        printf("Message written to memory: %s\n", data.message);
        data.turn = 1;
        memcpy(shmPtr, &data, sizeof(DataShared));
    };

    return 0;
}

void sigHandler(int i) {
    printf("Interrupt called");

    if (i == SIGINT) {
        if (shmdt(shmPtr) < 0) {
            perror("Failed to let go\n");
        }
    }

    exit(0);
}