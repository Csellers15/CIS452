#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
int main(int argc, char *argv[])
{
    if (argc < 5) {
        printf ("Usage: ln -option filename linkname\n");
        printf ("Options are\n-h\thard link\n-s\tsoft link\n");
        exit(1);
    }
    if (!strcmp(argv[1],"ln"))
    {
        if (!strcmp(argv[2],"-h"))
        {
            // Hard link
            if(link(argv[3], argv[4]) == -1)
                printf("Link Error\n");
        }
        else if  (!strcmp(argv[2],"-s"))
        {
            // Soft link
            if(symlink(argv[3], argv[4]) == -1)
                printf("Link Error\n");
        }
        else
            printf("Incorrect option.\n");
    }
    else 
        printf("Unknown Command.\n");
        
    return 0;
} 