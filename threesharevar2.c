//  实验6：多线程共享资源并发访问控制
//  一个进程中的多个线程对共享变量的非互斥并发访问会导致运行结果的不唯一。
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

int num = 200, count = 30;

void* sub1(void* arg)
{
    int i = 0, tmp;
    for(; i< count; i++)
    {
        printf("线程1：num操作前值为：%d; ", num);
        tmp = num - 1;
        printf("操作后tmp值为: %d; ", tmp);
        num = tmp;
        printf("操作后num值为：%d.\n", num);
    }
    return NULL;
}

void* sub2(void* arg)
{
     int i = 0, tmp;
    for(; i< count; i++)
    {
        printf("线程2：num操作前值为：%d; ", num);
        tmp = num - 1;
        printf("操作后tmp值为: %d; ", tmp);
        num = tmp;
        printf("操作后num值为：%d.\n", num);
    }
    return NULL;                      
}

int main(int argc, char** argv)
{
    pthread_t tid1, tid2;
    int err, i = 0, tmp;
    void *tret;
    err = pthread_create(&tid1, NULL, sub1, NULL);
    if(0!=err)
    {
        printf("pthread_create error: %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }
    err = pthread_create(&tid2, NULL, sub2, NULL);
    if(0!=err)
    {
        printf("pthread_create error: %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }
    for(; i< count; i++)
    {
        printf("main：num操作前值为：%d; ", num);
        tmp = num - 1;
        printf("操作后tmp值为: %d; ", tmp);
        num = tmp;
        printf("操作后num值为：%d.\n", num);
    }
    printf("两个线程运行结束\n");
    //阻塞等待线程退出
    err = pthread_join(tid1, &tret);
    if(0!=err)
    {
        printf("can not join with thread 1: %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }
    printf("Thread 1 exit code: %d\n", (int)tret);
    err = pthread_join(tid2, &tret);
    if(0!=err)
    {
        printf("can not join with thread 2: %s\n", strerror(err));
        exit(EXIT_FAILURE);
    }
    printf("Thread 2 exit code: %d\n", (int)tret);
    return EXIT_SUCCESS;
}