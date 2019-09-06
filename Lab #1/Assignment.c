
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>


int main()
{
    char *username, *host_info, *val;
    int uid, gid;

    username = getlogin();
    uid = getuid();
    gid = getgid();

    host_info = (char*)malloc(20* sizeof(char));
    gethostname(host_info, 20);

    printf("%s\n\n", username);
    printf("%i\n", uid);
    printf("%i\n\n", gid);
    printf("%s\n\n", host_info);

    struct passwd *p;
    for(int i = 0; i < 5; i++){
        p = getpwent();
        printf("%s\n", p->pw_name);
    }


    char env[50];
    printf("\nEnter Your Desired ENV variable:");
    scanf("%s", env);

    val = getenv(env);

    while(val == NULL) {
        printf("Not a valid Term\n");
        printf("Enter Your Desired ENV variable:");
        scanf("%s", env);

        val = getenv(env);
    }

    printf("val = %s\n", val);

    free(host_info);

    return 0;
}