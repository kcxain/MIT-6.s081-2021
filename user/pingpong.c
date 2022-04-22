#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int p[2];
  int pid;
  pipe(p);

  char buf[2];
  if(fork() == 0) {
      pid = getpid();
      if(read(p[0], buf, 1) == -1)
      {
        fprintf(2, "READ ERROR!\n");
        exit(1);
      }
      close(p[0]);
      
      printf("%d: received ping\n", pid);

      if(write(p[1], buf, 1) == -1){
        fprintf(2, "WRITE ERROR!\n");
        exit(1);
      }
      close(p[1]);
      exit(0);   
  } 
  else {
      pid = getpid();
      if(write(p[1], buf + 1, 1) == -1){
        fprintf(2, "WRITE ERROR!\n");
        exit(1);
      }
      close(p[1]);
      wait(0);
      if(read(p[0], buf + 1, 1) == -1)
      { 
        fprintf(2, "READ ERROR!\n");
        exit(1);
      }
      printf("%d: received pong\n", pid);
      close(p[0]);
      exit(0);
  }
}