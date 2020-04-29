//	实验6：共享变量加上互斥锁
//	互斥锁 pthread_mutex_t mylock 使各个线程对共享变量 num 的访问只能顺序进行，不能并发进行，结果唯一。
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>

int num = 200, count = 20;
pthread_mutex_t mylock = PTHREAD_MUTEX_INITIALIZER;

void *sub1(void *arg)
{
	int i = 0, tmp;
	for(; i < count; i++)
	{
		pthread_mutex_lock(&mylock);
		printf("thread 1: before operating num is %d, ", num);
		tmp = num - 1;
		printf("after operating tmp is %d, ", tmp);
		num = tmp;
		printf("after operating num is %d.\n", num);
		pthread_mutex_unlock(&mylock);
	}
	return NULL;
}

void *sub2(void *arg)
{
	int i = 0, tmp;
	for(; i < count; i++)
	{
		pthread_mutex_lock(&mylock);
		printf("thread 2: before operating num is %d, ", num);
		tmp = num - 1;
		printf("after operating tmp is %d, ", tmp);
		num = tmp;
		printf("after operating num is %d.\n", num);
		pthread_mutex_unlock(&mylock);
	}
	return NULL;
}

int main(int argc, char** argv)
{
	pthread_t tid1, tid2;
	int err, i = 0, tmp;
	void *tret;
	err = pthread_create(&tid1, NULL, sub1, NULL);
	if (0 != err)
	{
		printf("pthread_create error: %s\n", strerror(err));
		exit(EXIT_FAILURE);
	}
	err = pthread_create(&tid2, NULL, sub2, NULL);
	if(0 != err)
	{
		printf("pthread_create error: %s\n", strerror(err));
		exit(EXIT_FAILURE);
	}
	for(; i < count; i++)
	{
		pthread_mutex_lock(&mylock);
		printf("main function: before operating num is %d, ", num);
		tmp = num - 1;
		printf("after operating tmp is %d, ", tmp);
		num = tmp;
		printf("after operating num is %d.\n", num);
		pthread_mutex_unlock(&mylock);
	}
	printf("join all threads\n");
	err = pthread_join(tid1, &tret);
	if(0 != err)
	{
		printf("can not join with thread 1: %s\n", strerror(err));
		exit(EXIT_FAILURE);
	}
	printf("thread 1 exit code %d\n", (int)tret);
	err = pthread_join(tid2, &tret);
	if(0 != err)
	{
		printf("can not join with thread 2: %s\n", strerror(err));
		exit(EXIT_FAILURE);
	}
	printf("thread 2 exit code %d\n", (int)tret);
	return EXIT_SUCCESS;
}