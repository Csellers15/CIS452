#include <pthread.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>


#define MAX_THREADS 100

int filesNum = 0; 
int threadsServed = 0;

void* getFile(void* arg);
void sigHandle(int sigNum);

int main(){
    pthread_t threads[MAX_THREADS];
    int status; 
    char input[256];

    char* fileInput = input;
    int threadCount = 0; 

    signal(SIGINT, sigHandle);

    while(1){
        printf("Enter a file name: ");
        scanf("%s", fileInput);

        if ((status = pthread_create(&threads[threadCount], NULL,  getFile, (void*)fileInput)) != 0) {
            fprintf (stderr, "thread create error %d: %s\n", status, strerror(status));
            exit (1);
        }   

        if((status = pthread_detach(threads[threadCount])) != 0){
            fprintf (stderr, "thread detach error %d: %s\n", status, strerror(status));
            exit (1);
        }

        if(threadCount < MAX_THREADS){
            threadCount++;
        } else {
            threadCount = 0;
        }

        filesNum = filesNum + 1;
    }

    return 0; 

}

void* getFile(void* arg) {
    char* usrstring = malloc(strlen( (char*)arg )+1 );
    strcpy(usrstring, (char*)arg);

    //generate 80/20 probability
    srand(time(NULL));
    float random = (float) rand()/RAND_MAX;
    if(random < 0.8) {
        sleep(1);
    } else {
        sleep((7 + rand() % (10 + 1 - 7)));
    }

    printf("\n Thread with String: %s\n", usrstring);
    threadsServed = threadsServed + 1;

    free(usrstring);
    return NULL;
}

void sigHandle(int sigNum){
    if(sigNum == SIGINT){
        printf("otal number of file requests received %d\n", filesNum);
        printf("Exiting.\n");
        exit(0);
    }
}


