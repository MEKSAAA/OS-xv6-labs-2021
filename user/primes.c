#include "kernel/types.h"
#include "user/user.h"

void
filter(int p[2])
{
  int prime;
  int n;
  int newp[2];

  // 读取并打印第一个数字，即素数
  close(p[1]);
  if(read(p[0], &prime, sizeof(prime)) == 0)
    exit(0);
  printf("prime %d\n", prime);

  // 创建新的管道并派生新进程
  pipe(newp);
  if(fork() == 0){
    close(p[0]);
    filter(newp);
  } else {
    close(newp[0]);
    while(read(p[0], &n, sizeof(n))){
      if(n % prime != 0)
        write(newp[1], &n, sizeof(n));
    }
    close(p[0]);
    close(newp[1]);
    wait(0);
    exit(0);
  }
}

int main(int argc, char *argv[])
{
  int p[2];
  int i;

  pipe(p);

  if(fork() == 0){
    filter(p);
  } else {
    close(p[0]);
    for(i = 2; i <= 35; i++){
      write(p[1], &i, sizeof(i));
    }
    close(p[1]);
    wait(0);
    exit(0);
  }
  return 0;
}
