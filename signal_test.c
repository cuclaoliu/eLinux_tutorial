//  实验：Linux 信号实验
//  使用 kill 发送 SIGALRM 信号
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

int interrupt_time = 0; //  记录收到信号次数
int child_giveup = 0;   
struct sigaction act; //  声明信号处理结构体变量

void play_game(int sig) {
  interrupt_time++;
  if (interrupt_time < 3) {
    printf("子：我再玩一次！\n");
  } else { //被催3次，看心情是否放弃
    srand((unsigned)time(NULL));
    child_giveup = rand() % 2;
  }
}

int main(int argn, char **argv) {
  pid_t childpid;
  int status;
  int retval;
  int count = 0;
  // fork()函数通过系统调用创建一个与原来进程几乎完全相同的进程
  childpid = fork();
  if (-1 == childpid) {
    perror("fork()");
    return (EXIT_FAILURE);
  } else if (0 == childpid) {
    printf("子进程更改 SIGTERM 处理，每秒游戏。\n");
    act.sa_handler = play_game;
    //创建空的信号屏蔽字
    sigemptyset(&act.sa_mask);
    //安装 SIGTERM 信号接收时执行
    sigaction(SIGTERM, &act, 0);
    while (!child_giveup) {
      printf("子：游戏中 %d。\n", ++count);
      sleep(1);
    }
    printf("子：好啦，怕了你啦！\n");
    printf("子进程退出。\n");
  } else {
    sleep(1); //父进程睡眠，等子进程更改信号处理
    while (0 == (waitpid(childpid, &status, WNOHANG))) // WNOHANG
    {
      printf("父：快去写作业 %d！\n", ++count);
      if (count <= 3)
        kill(childpid, SIGTERM);
      else {
        kill(childpid, SIGKILL);
        sleep(1);
        printf("父夺走了手机！\n");
      }
      sleep(1);
    }
    printf("父进程退出。\n");
  }
  return (EXIT_SUCCESS);
}
