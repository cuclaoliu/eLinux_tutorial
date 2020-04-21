//  实验5：结果不唯一的多线程并发运行实例
//  to compile: gcc create-threads.c -o create-threads -lpthread
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void* printyou(void* unused)
{
    int c = 2000;
    while (c--)
        printf("你%d", c);
    return NULL;
}

void* printme(void* unused)
{
    int c = 2000;
    while (c--) printf("我%d", c);
    return NULL;
}

void* printhim(void* unused)
{
    int c = 2000;
    while (c--) printf("他%d", c);
    return NULL;
}

int main(int argc, char** argv)
{
    int c = 2000;
    pthread_t thread_id1, thread_id2, thread_id3;
    pthread_create(&thread_id1, NULL, &printyou, NULL);
    pthread_create(&thread_id2, NULL, &printme, NULL);
    pthread_create(&thread_id3, NULL, &printhim, NULL);
    while (c--) printf("主%d", c);
    return EXIT_SUCCESS;
}