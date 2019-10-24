# include <assert.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <string.h>
# include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>


int getpagesize(void);

int main(int argc, char const *argv[])
{
    /* Page size (bytes) */ 
    long sz = sysconf(_SC_PAGESIZE);
    printf("%ld \n", sz);

    /* Physical pages in system */
    long pg = sysconf(_SC_PHYS_PAGES);
    printf("%lu\n", pg);
    return 0;
}


//For the Maximum # of open files: soft limit
int main() {
    int test;

    while(1){
        test = open("test", O_RDONLY);
        if (test < 0){
            perror("open");
            exit(1);
        }
        printf("%d: ok\n", test);
    }
}


//Maximum shared memory Size (This took too long)
#define FOO 25267159099

int main() {
    int shmId;
    if ((shmId = shmget (IPC_PRIVATE, FOO, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
        perror ("Can't get\n");
        exit (1);
    }
    if (shmctl (shmId, IPC_RMID, 0) < 0) {
        perror ("Can't delete\n");
        exit(1);
    }
    printf("success\n");
    return 0;
}
    