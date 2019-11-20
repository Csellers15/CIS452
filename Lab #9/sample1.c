#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 16

int main()
{

    char *data1, *buffer;

    data1 = malloc (SIZE);
    buffer = malloc (SIZE);

    printf ("Please input username: ");
    
    scanf("%s", buffer);
   
    int length = 0;

    for(int i = 0; i < strlen(buffer);i++){
        length++;   

        if(length < SIZE)
            data1[i] = buffer[i];
        else{ 
            printf("input too long\n");
            exit(1);
        }
    }
    printf ("you entered: %s\n", data1);

    free(buffer);
    free (data1);
    return 0;

}


