#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <signal.h> 
#include <string.h>
#include <unistd.h>

#define FOO 4096

void sigHandler(int);

int shmId;
char *shmPtr;

int main ()
{
   
   key_t key = ftok("shmkey", 65);
   signal(SIGINT, sigHandler);
   if ((shmId = shmget (key, FOO, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
      perror ("i can't get no..\n");
      exit (1);
   }
   if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
      perror ("can't attach\n");
      exit (1);
   }

   while (1) {

      if (!strcmp(shmPtr,"")) {
         //Get user input
         printf("Enter text \n");
         fgets(shmPtr, 100, stdin);

         // Wait until SM empty
         while(strcmp(shmPtr,""));
      }
   }
   return 0;
} 


void sigHandler(int sigNum)
{
   printf("Detaching.\n");
   if (shmdt (shmPtr) < 0) {
      perror ("just can't let go\n");
      exit (1);
   }
   printf("Deallocating.\n"); 
   if (shmctl (shmId, IPC_RMID, 0) < 0) {
      perror ("can't deallocate\n");
      exit(1);
   }
   exit(0);
}