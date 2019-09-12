#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>
#include <sys/resource.h> 
#include <sys/time.h>
#include <sys/wait.h>

int main(){

    int totalMicro = 0;
    int totalSec = 0; 

    while(1){
        pid_t pid; 
        int status; 
        int cmdLen;
        struct rusage stats; 
        int totalSwitches = 0;

        char input[256];
        char *params[10];

        printf("Please Enter Your Command(Or quit to exit): ");
        fgets(input, 256 ,stdin);

        while(input[0] == '\n' || input[0] == ' ')
            fgets(input, 256 ,stdin);

        cmdLen = strlen(input);
        if(cmdLen > 0 && input[cmdLen-1] == '\n') input[cmdLen - 1] = '\0';
        
        char *tokenize = strtok(input, " ");
        for(int i = 0; i < sizeof(tokenize); i++) {
            params[i] = tokenize;
            tokenize = strtok(NULL, " ");
        }

        if (strstr(params[0], "quit")) exit(0);


        pid = fork();

        if(pid < 0){
            printf("error");
            exit(1);
        } else if (pid){
            waitpid(-1, &status, 0);
            getrusage(RUSAGE_CHILDREN, &stats); 
            printf("User CPU usage: %ld.%06ld sec\n", (stats.ru_utime.tv_sec - totalSec), (stats.ru_utime.tv_usec - totalMicro));
            totalSec = stats.ru_utime.tv_sec;
            totalMicro = stats.ru_utime.tv_usec;
            printf("Context Switches: %ld\n", stats.ru_nivcsw - totalSwitches);
            totalSwitches = stats.ru_nivcsw;
        } else {
            execvp(params[0], params);
            exit(0);
        }
    }
}