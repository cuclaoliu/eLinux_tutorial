//  实验：多线程共享资源并发访问控制
//  一个进程中的多个线程对共享变量的非互斥并发访问会导致运行结果的不唯一。
//  To compile: gcc bank.c -o bank -lpthread
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

int balance = 0;
int times = 1e5;

void* deposit(void* arg)
{
    int i = 0;
    for(i=0; i < times; i++)
	//printf("%dth deposit, balance: %d\n", i, ++balance);
	++balance;
    printf("balance = %d after depositting %d times.\n", balance, i);
    return NULL;
}

void* withdraw(void* arg)
{
    int i = 0;
    for(i=0; i< times; i++)
	//printf("%dth withdraw 1, balance: %d\n", i, --balance);
	--balance;
    printf("balance = %d after withdrawing %d times.\n", balance, i);
    return NULL;
}

int main(int argc, char** argv)
{
    if (argc == 2)
	times = atoi(argv[1]);
    pthread_t tid1, tid2;
    void *tret;
    if (0 != pthread_create(&tid1, NULL, deposit, NULL)) {
        printf("pthread_create error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (0 != pthread_create(&tid2, NULL, withdraw, NULL)){
        printf("pthread_create error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    //阻塞等待线程退出
    if (0 != pthread_join(tid1, &tret)){
        printf("can not join with thread 1: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (0 != pthread_join(tid2, &tret)) {
        printf("can not join with thread 2: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    printf("All done: balance = %d\n", balance);
    return EXIT_SUCCESS;
}
