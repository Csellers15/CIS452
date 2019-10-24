# include <assert.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/types.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <string.h>
# include <fcntl.h>

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
    