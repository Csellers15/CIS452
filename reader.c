#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <string.h>
#include <unistd.h>

#define FOO 4096

int shmId;
char *shmPtr;

int main ()
{
   
   key_t key = ftok("shmkey", 65);
   char *buff;

   if ((shmId = shmget (key, FOO, IPC_CREAT|S_IRUSR|S_IWUSR)) < 0) {
      perror ("i can't get no..\n");
      exit (1);
   }
   if ((shmPtr = shmat (shmId, 0, 0)) == (void*) -1) {
      perror ("can't attach\n");
      exit (1);
   }
   while(1) {

      if (strcmp(shmPtr,"")) {
         //Read from shared memory
         buff = shmPtr;
         printf("Read from SM: %s \n", buff);

         // Empty the SM
         strcpy(shmPtr, "");
      }
      
   }
   printf("Detaching.\n");
      if (shmdt (shmPtr) < 0) {
         perror ("just can't let go\n");
         exit (1);
      }
   return 0;
} 
