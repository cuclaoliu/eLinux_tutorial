//  实验4：Linux 进程控制实验
//  使用 kill 终止进程，使用 sleep 让进程自己睡眠，使用 waitpid 等待子进程结束。
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

int main(int argn, char **argv) {
  pid_t childpid;
  int status, retval;
  int sleep_time;
  int count = 0;
  // fork()函数通过系统调用创建一个与原来进程几乎完全相同的进程
  childpid = fork();
  srand((unsigned) time(NULL) ^ getpid());
  sleep_time = rand() % 8;
  if (-1 == childpid) {
    perror("fork()");
    return (EXIT_FAILURE);
  } else if (0 == childpid) { // In child process
    printf("子进程 %d，睡眠%d秒退出，不知退出前会不会被杀死\n", (int)getpid(), sleep_time);
    count++;
    sleep(sleep_time);//可能会在退出前被父进程杀死
  } else {
    printf("父进程 %d，睡眠%d秒后，查看子进程结束否，未结束，杀\n",
           (int)getpid(), sleep_time);
    count--;
    sleep(sleep_time);
    if (0 == (waitpid(childpid, &status, WNOHANG))) // WNOHANG
    {
      printf("杀死子进程前\n");
      retval = kill(childpid, SIGKILL);
      printf("父进程杀死子进程\n");
      if (retval) {
        perror("kill()");
        waitpid(childpid, &status, 0);
      } else {
        printf("Process %d is killed.\n", childpid);
        printf("杀死子进程后\n");
      }
    }else{
        printf("还没动手呢，子进程已经退出了\n");
    }
  }
  printf("现在退出：count = %d\n", count);
  return (EXIT_SUCCESS);
}
