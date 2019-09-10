#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>
#include <sys/types.h>

int main(){

    while(1){
        pid_t pid; 
        int status; 
        int cmdLen;

        char input[256];
        char *params[10];

        printf("Please enter Your Command: ");
        fgets(input, 256 ,stdin);

        while(input[0] == '\n' || input[0] == ' ')
            fgets(input, 256 ,stdin);
        
        cmdLen = strlen(input);
        if(cmdLen > 0 && input[cmdLen-1] == '\n')
            input[cmdLen - 1] = '\0';
        
        char *tokenize = strtok(input, " ");
        for(int i = 0; i < sizeof(tokenize); i++) {
            params[i] = tokenize;
            tokenize = strtok(NULL, " ");
        }

        pid = fork();

        if(pid < 0){
            printf("error");
            exit(1);
        }

        else if (pid){
            waitpid(-1, &status, 0);
        }

        else {
            execvp(params[0], params);
            exit(0);
        }
    }

}