#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

int main()
{
    DIR *dirPtr;
    struct dirent *entryPtr;
    struct stat size;

    dirPtr = opendir (".");



    while ((entryPtr = readdir (dirPtr))){
        printf ("File Name: %-20s\n", entryPtr->d_name);
        stat(entryPtr->d_name, &size);
        printf("Size is: %li Bytes\n", size.st_size);
    }

    closedir (dirPtr);
    return 0;
} 