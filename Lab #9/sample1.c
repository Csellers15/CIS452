#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 16

int main()
{

    char *data1 = calloc(1,1), buffer[SIZE];
    printf ("Please input username: ");
    while( fgets(buffer, SIZE , stdin)){
        data1 = realloc( data1, strlen(data1)+1+strlen(buffer) );
        if( !data1 ) 
            strcat(data1, buffer);
        printf ("Enter ctrl-D to break loop\n");
    }

    printf ("you entered: %s", data1);
    free(data1);
}


