/***** testwrite.c *******/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/types.h>
#include <fcntl.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define SHMSIZE 4096
#define SEMAPHORE_NAME "my_semaphore"

typedef struct {
  int score;
  int age;
} people;   /*用于共享的数据结构*/

int main(int argc, char **argv) {
  int shm_id, i;
  sem_t* semaphore;
  key_t shmkey;
  char temp;
  people *p_map;    /*用于共享的数据指针*/
  const char *name = "./tmp";   //一个存在的文件或目录
  shmkey = ftok(name, '0');     //生成一个整数的 key，也可以自己定
  if (shmkey == -1)
    perror("ftok error");
//创建共享内存和信号量的IPC
    semaphore = sem_open(SEMAPHORE_NAME, O_CREAT, 0644, 1);
    if(semaphore == SEM_FAILED){
        perror("semaphore error");
        sem_unlink(SEMAPHORE_NAME);
        return EXIT_FAILURE;
    }
  //创建共享内存
  shm_id = shmget(shmkey, SHMSIZE, IPC_CREAT|0666);
  if (shm_id == -1) {
    perror("shmget error");
    return (EXIT_FAILURE);
  }
  //映射 shared_memory 到当前进程的地址当中，之后对进程中地址 addr 操作就是对共享内存操作
  p_map = (people *)shmat(shm_id, NULL, 0);
  if (p_map == (people*)-1){
    perror("shmat error");
    return EXIT_FAILURE;
  }
  temp = 'a';
  for (i = 0; i < SHMSIZE/sizeof(people); i++) {
    srand((unsigned)time(NULL));
    sem_wait(semaphore);
    //写 shared_memory
    (*(p_map + i)).age = 18 + rand()%4;
    (*(p_map + i)).age = 18 + rand()%4;
  }
  //关闭共享内存
  if (shmdt(p_map) == -1) {
    perror(" detach error ");
    return (EXIT_FAILURE);
  }
  return (EXIT_SUCCESS);
}