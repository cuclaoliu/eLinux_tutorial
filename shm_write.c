/***** testwrite.c *******/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <unistd.h>
#define SHMSIZE 4096
typedef struct {
  char name[4];
  int age;
} people;   /*用于共享的数据结构*/

int main(int argc, char **argv) {
  int shm_id, i;
  key_t key;
  char temp;
  people *p_map;    /*用于共享的数据指针*/
  const char *name = "./tmp";
  key = ftok(name, 0);
  if (key == -1)
    perror("ftok error");
  //创建共享内存
  shm_id = shmget(key, SHMSIZE, IPC_CREAT|0666);
  if (shm_id == -1) {
    perror("shmget error");
    return (EXIT_FAILURE);
  }
  //映射 shared_memory
  p_map = (people *)shmat(shm_id, NULL, 0);
  temp = 'a';
  for (i = 0; i < 10; i++) {
    temp += 1;
    //写 shared_memory
    memcpy((*(p_map + i)).name, &temp, 1);
    (*(p_map + i)).age = 20 + i;
  }
  //关闭共享内存
  if (shmdt(p_map) == -1) {
    perror(" detach error ");
    return (EXIT_FAILURE);
  }
  return (EXIT_SUCCESS);
}