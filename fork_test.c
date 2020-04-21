#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int global_var = 1;
int main(int argc, char *argv[]) {
  int i = 0;
  int local_var = 2;
  printf("i\tson/pa\tglobal\tlocal\tppid\tpid\tfpid\n");
  // ppid指当前进程的父进程pid，对应的值由 getppid() 获得
  // pid指当前进程的pid，对应的值由 getpid() 获得
  // fpid指fork返回给当前进程的值
  printf("before fork()\n");
  printf("%d\t%s\t%d\t%d\t%4d\t%4d\t%4d\n", i, "xxxx",
         global_var, local_var, getppid(), getpid(), 0);
  for (i = 0; i < 2; i++) {
    pid_t fpid = fork();
    //从这里开始，就fork了新的子进程（接下来，父子进程都在循环中，都还会 fork）
    if (fpid == -1) { // fork失败时返回-1
      printf("fork failed!\n");
      return EXIT_FAILURE;
    } else if (fpid == 0) { //子进程返回0
      //sleep(2);
      printf("I'm a newborn process.\n");
      global_var *= 10;
      local_var *= 10;
    } else { //父进程返回子进程PID
        printf("I have forked process with PID=%d\n", fpid);
      sleep(1);
    }
    //父子进程都会执行
    printf("%d\t%s\t%d\t%d\t%4d\t%4d\t%4d\n", i, 0 == fpid ? "child" : "parent",
           global_var, local_var, getppid(), getpid(), fpid);
  }
  printf("global variable address: (%p)=%d\n", &global_var, global_var);
  printf("local variable address: (%p)=%d\n", &local_var, local_var);
  printf("PID %d done!\n", getpid());
  return EXIT_SUCCESS;
}
