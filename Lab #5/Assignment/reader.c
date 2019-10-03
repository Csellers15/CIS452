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
int mId;
char *mPtr;

typedef struct {
	int turn;
	char message[1024];
	int count; 
} DataShared;

int main() {
	DataShared data;
	data.turn = 0;
	signal(SIGINT, sigHandler);

	//generates key
	key = ftok("mkey",65);

  	//returns an identifier in mId
	if ((mId = shmget(key, SIZE, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0){
    perror("shared memory error");
    exit(1);
	}

  	// shmat to attach to shared memory
	if((mPtr = shmat(mId, 0, 0)) == (void*) -1) {
    perror("Can't attach\n");
    exit(1);
	}

	data.count = 0;

	while(1) {
		// request critical section
		while(!data.turn) {
		//not time for the reader, check if token is changed.
			memcpy(&data, mPtr, sizeof(DataShared));
		}

		data.count++;

		usleep(1);
		fprintf(stderr, "Read from memory: %s\n", data.message);
		usleep(1);
	

		data.count--;

		while(data.count > 0){
			;
		}
		
		data.turn = 0;
		memcpy(mPtr, &data, sizeof(DataShared));
	};

	return 0;
}


void sigHandler(int i) {
	printf("Interrupted");

	if (shmctl(mId, IPC_RMID, NULL) < 0) {
		perror("What??? We can't deallocate?!?! RUN, RUN NOW!!!");
		exit(1);
	}
	exit(0);
}