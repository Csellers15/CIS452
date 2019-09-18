
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 
#include <signal.h>
#include <sys/wait.h>


void sigHandler (int);
void sigChildhandler();

int main(){
	pid_t pid, ppid;
	int status;

	signal(SIGINT, sigHandler);
	signal(SIGUSR1, sigHandler);
	signal(SIGUSR2, sigHandler);


	pid = fork();
	if(pid < 0) {
		printf("Fork Failed");
		exit(1);
	} else if( pid == 0){
		ppid = getppid();
		signal(SIGINT, sigChildhandler);
		while(1){
			int randSleep  = rand() % (5) + 1;
			sleep(randSleep);
			int whoSleeps = rand() % 2 == 0 ? SIGUSR1 : SIGUSR2;
			kill(ppid, whoSleeps);
		}
	} 

	
	printf("parent spawned child PID# %d\nwaiting...   ", pid);
	waitpid(-1, &status, 0);

	return 0;
}

void sigChildhandler(){
	exit(0);
}

void sigHandler (int sigNum){
	if(sigNum == SIGINT){
		printf("Recieved and interrupt, Shutting Down\n");
		exit(0);
	}
	else if(sigNum == SIGUSR1)printf("Recieved a signal from SIGUSR1\nwaiting...   ");
	else printf("Recieved a signal from SIGUSR2\nwaiting...   ");
}