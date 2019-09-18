
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <signal.h>
#include <sys/wait.h>


int main(){
    for(int i = 0; i < 20; i++){
        int randSleep  = rand() % 2 == 0 ? SIGUSR1 : SIGUSR2;
        printf("randSleep = %i\n", randSleep);
    }

    for(int i = 0; i < 20; i++){
        int randSleep  = rand() % 2 == 0 ? SIGUSR1 : SIGUSR2;
        printf("randSleep = %i\n", randSleep);
    }

}