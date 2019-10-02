#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define FOO 4096

void sigHandler(int);

key_t key;
int shmId;
char *shmPtr;
typedef struct {
    int turn;
    char message[1024];
} SharedData;

int main() {
	SharedData sharedData;
	sharedData.turn = 0;
	signal(SIGINT, sigHandler);

	key = ftok("shmkey",65); 

	if ((shmId = shmget(key, FOO, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0){
		perror("shared memory error");
		exit(1);
	} 

	if((shmPtr = shmat(shmId, 0, 0)) == (void*) -1) {
		perror("Can't attach\n");
		exit(1);
	}

	while(1) {
		while(!sharedData.turn) {
			memcpy(&sharedData, shmPtr, sizeof(SharedData));
		}

		printf("Read from memory: %s\n", sharedData.message);

		sharedData.turn = 0;
		memcpy(shmPtr, &sharedData, sizeof(SharedData));
	}

	return 0; 
}

void sigHandler(int sigNum){
    printf("Shutting Down\n");


	if (shmdt (shmPtr) < 0) {
		perror ("Cant Detach\n");
		exit (1);
	}


    if (shmctl (shmId, IPC_RMID, 0) < 0) {
        perror ("can't deallocate\n");
        exit(1);
    }
    
	exit(0);
}