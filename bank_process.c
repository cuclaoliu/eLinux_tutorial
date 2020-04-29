//  实验：多进程 System V 共享内存并发访问控制，利用POSIX信号量互斥

#include <errno.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char **argv) {
  pid_t pid;
  int shmid, i;
  const char *SEM_NAME = "my_semaphore_name";
  sem_t *mysem;
  int *balance;
  //创建一个 POSIX 有名信号量，初始值为1
  if ((mysem = sem_open(SEM_NAME, O_CREAT, 0666, 1)) == SEM_FAILED) {
    perror("sem_open failed");
    return EXIT_FAILURE;
  }

  //创建一个 System V 共享内存id
  if ((shmid = shmget(IPC_PRIVATE, sizeof(int), IPC_CREAT | 0666)) == -1) {
    perror("shmget error!");
    return EXIT_FAILURE;
  }
  pid = fork(); //创建子进程
  if (pid < 0) {
    perror("fork error!");
  } else if (pid == 0) {
    if ((balance = (int *)shmat(shmid, NULL, 0)) == (int *)-1) {
      perror("shmat error!");
      return EXIT_FAILURE;
    }
    for (i = 0; i < 1e6; i++) {
      //若信号量为0，阻塞，否则减1
      sem_wait(mysem);
      --(*balance);
      //信号量增1，可唤醒阻塞在该信号量的进程/线程
      sem_post(mysem);
    }
    printf("Child Process: shmid=%d, balance=%d\n", shmid, *balance);
    if (shmdt(balance) == -1) {
      perror("detach error!");
      return EXIT_FAILURE;
    }
  } else {
    balance = (int *)shmat(shmid, NULL, 0);
    if (balance == (int *)-1) {
      perror("shmat error!");
      return EXIT_FAILURE;
    }
    for (i = 0; i < 1e6; i++) {
      sem_wait(mysem);
      ++(*balance);
      sem_post(mysem);
    }
    printf("Father Process: shmid=%d, balance=%d\n", shmid, *balance);
    waitpid(pid, NULL, 0); //等待子进程退出
    sem_unlink(SEM_NAME);
    printf("Father Process: balance=%d(after child process exit)\n", *balance);
    if (shmdt(balance) == -1) {
      perror("detach error!");
      return EXIT_FAILURE;
    }
  }
  return EXIT_SUCCESS;
}
