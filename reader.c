#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

void sigHandler(int);

key_t key;
int mId;
char *mPtr;

typedef struct {
	int turn;
	char message[1024];
} DataShared;

int main() {
	DataShared data;
	data.turn = 0;
	signal(SIGINT, sigHandler);

	//generates key
	key = ftok("mkey",65);

  	//returns an identifier in mId
	if ((mId = shmget(key, 4096, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0){
    perror("shared memory error");
    exit(1);
	}

  	// shmat to attach to shared memory
	if((mPtr = shmat(mId, 0, 0)) == (void*) -1) {
    perror("Can't attach\n");
    exit(1);
	}

	while(1) {
		while(!data.turn) {
			memcpy(&data, mPtr, sizeof(DataShared));
		}

		usleep(1);
		printf("Read from memory: %s\n", data.message);
		usleep(1);

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