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
	signal(SIGINT, sigHandler);

	key = ftok("mkey",65);

	if ((shmId = shmget(key, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0){
		perror("shared memory error");
		exit(1);
	}

	if((shmPtr = shmat(shmId, 0, 0)) == (void*) -1) {
		perror("Can't attach\n");
		exit(1);
	}

	while(1) {
		while(!data.turn) {
			memcpy(&data, shmPtr, sizeof(DataShared));
		}

		data.count++;
		// memcpy(shmPtr, &data, sizeof(DataShared));

		usleep(1);
		fprintf(stderr, "Read from memory: %s\n", data.message);
		usleep(1);
	

		data.count--;
		// memcpy(shmPtr, &data, sizeof(DataShared));

		while(data.count > 0){
			;
		}
		
		data.turn = 0;
		memcpy(shmPtr, &data, sizeof(DataShared));
	};

	return 0;
}


void sigHandler(int i) {
	printf("Interrupted");

	if (shmctl(shmId, IPC_RMID, NULL) < 0) {
		perror("What??? We can't deallocate?!?! RUN, RUN NOW!!!");
		exit(1);
	}
	exit(0);
}