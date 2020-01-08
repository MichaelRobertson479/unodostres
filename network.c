#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <dirent.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <time.h>

#define NUMOFPLAYERS 12345

void init() {

int shmd;
  int* data;
  shmd = shmget(NUMOFPLAYERS, sizeof(int), IPC_CREAT | IPC_EXCL | 0644);

if (shmd == -1) {
printf("Segment already exists\n");
shmd = shmget(NUMOFPLAYERS, sizeof(int), 0644);
  data = shmat(shmd, 0, 0);
printf("Number of players: %d\n", *data);
}

else {
printf("Segment created\n");
data = shmat(shmd, 0, 0);
printf("Input number of players: ");
//fgets(data, sizeof(int), stdin);
*data = 4;
}
}

int main() {

init();

return 0;
}