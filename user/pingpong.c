#include "kernel/types.h"
#include "user/user.h"

int main() {
  int p1[2], p2[2];
  char buf;

  // 创建两个管道
  pipe(p1);
  pipe(p2);

  if (fork() == 0) {
    // 子进程
    close(p1[1]); // 关闭父进程写端
    close(p2[0]); // 关闭子进程读端

    read(p1[0], &buf, 1);
    printf("%d: received ping\n", getpid());
    write(p2[1], &buf, 1);

    close(p1[0]);
    close(p2[1]);
  } else {
    // 父进程
    close(p1[0]); // 关闭子进程读端
    close(p2[1]); // 关闭父进程写端

    write(p1[1], "A", 1);
    read(p2[0], &buf, 1);
    printf("%d: received pong\n", getpid());

    close(p1[1]);
    close(p2[0]);
  }

  exit(0);
}
