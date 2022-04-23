#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void 
func(int r)
{
  int num;
  //递归终止
  if(read(r, &num, sizeof(int)) == 0){
    exit(0);
  }
  int p[2];
  pipe(p);
  int pid;
  if((pid = fork()) != 0){
    close(p[0]);
    printf("prime %d\n", num);
    int n = num;
    while(read(r, &num, sizeof(int)) != 0){
      if(num % n){
        write(p[1], &num, sizeof(int));
      }
    }
    close(p[1]);
    wait(0);
    exit(0);
  } else{
    close(p[1]);
    func(p[0]);
    close(p[0]);
  }
}
int
main(int argc, char *argv[])
{
  //p[0]读
  //p[1]写
  int p[2];
  pipe(p);
  int pid;
  if((pid = fork()) != 0){
    close(p[0]);
    //第一个进程，把所有数送进第一个pipe
    for(int i = 2; i <= 35; i++){
        write(p[1], &i, 4);
    }
    close(p[1]);
    wait(0);
  }
  else{
    close(p[1]);
    //参数传入前一个pipe的读端口
    func(p[0]);
    close(p[0]);
  }
  exit(0);
}