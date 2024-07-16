#include "kernel/types.h"
#include "user/user.h"
#include "kernel/param.h"

#define BUFFER_SIZE 512

void run_command(char *cmd, char **argv, int argc) {
  if (fork() == 0) {
    // 在子进程中执行命令
    exec(cmd, argv);
    exit(0);
  } else {
    // 在父进程中等待子进程完成
    wait(0);
  }
}

int main(int argc, char *argv[]) {
  char buffer[BUFFER_SIZE];
  int n, i;
  char *args[MAXARG];

  // 检查参数个数
  if (argc < 2) {
    fprintf(2, "Usage: xargs command [initial-args]\n");
    exit(1);
  }

  // 将命令和初始参数复制到 args 数组
  for (i = 1; i < argc; i++) {
    args[i - 1] = argv[i];
  }
  int initial_argc = argc - 1;

  while ((n = read(0, buffer, sizeof(buffer))) > 0) {
    int buf_idx = 0;
    while (buf_idx < n) {
      // 找到一行的结束
      int start = buf_idx;
      while (buf_idx < n && buffer[buf_idx] != '\n') {
        buf_idx++;
      }

      // 将行内容添加到 args 数组
      buffer[buf_idx] = '\0'; // 替换 '\n' 为 '\0'
      buf_idx++;

      args[initial_argc] = &buffer[start];
      args[initial_argc + 1] = 0; // 确保 args 以 null 结尾

      // 运行命令
      run_command(argv[1], args, initial_argc + 1);
    }
  }

  exit(0);
}
