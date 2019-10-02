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
		int flag;
		char message[1024];
	} SharedData;

int main() {
	SharedData sharedData;
	sharedData.flag = 0;
	signal(SIGINT, sigHandler);

	key = ftok("shmkey",65); 

	if((shmId = shmget(key, FOO, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0 ) {
		perror("Error creating shared memory\n");
		exit(1);
	} 

	if((shmPtr = shmat(shmId, 0, 0)) == (void*) -1) {
		perror("Can't attach\n");
		exit(1);
	}

	while(1) {
		while (sharedData.flag) {
			memcpy(&sharedData, shmPtr, sizeof(SharedData));
		}

		printf("Enter a message: \n" ); 
		scanf("%s", sharedData.message);

		printf("Message written to memory: %s\n", sharedData.message);
		sharedData.flag = 1;
		memcpy(shmPtr, &sharedData, sizeof(SharedData));
	}

	return 0; 
}


void sigHandler(int sigNum) {
	printf("Shutting Down...\n");

	if (shmdt (shmPtr) < 0) {
		perror ("just can't let go\n");
		exit (1);
	}
	if (shmctl (shmId, IPC_RMID, 0) < 0) {
		perror ("can't deallocate\n");
		exit(1);
	}
	exit(0);
}