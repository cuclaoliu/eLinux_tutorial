//  实验：Linux 管道通信
//  pipe.c 父子进程之间无名管道通信

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#define BUFFER_SIZE 1024
#define DELAY_TIME 1

int main(int argc, char** argv)
{
    int pipe_fds[2];        /* 两个文件描述符 */
    pid_t pid;
    int ret_size;
    char buff[BUFFER_SIZE];
    const char data[] = {"pipe test 1234"};

    memset((void*)buff, 0, sizeof(buff));   /*初始化 buff 为0*/
    if(pipe(pipe_fds)<0){
        printf("Fail to create pipe.\n");
        exit(EXIT_FAILURE);
    }
    if((pid = fork())==(pid_t)0) {  //pid==0为子进程
        close(pipe_fds[0]);         //关闭管道读（0入1出）
        sleep(DELAY_TIME);          //等待父进程关闭写端
        if((ret_size=write(pipe_fds[1], data, strlen(data)))!=-1) {
            printf("%d bytes wrote into the pipe: '%s'.\n", ret_size, data);
        }
        close(pipe_fds[1]);
    } else if(pid > 0) {            //pid>0为父进程
        close(pipe_fds[1]);         //关闭管道写端
        sleep(DELAY_TIME*3);        //等待子进程关闭文件读端
        if((ret_size=read(pipe_fds[0], buff, BUFFER_SIZE))>0){
            printf("%d bytes readed from the pipe: '%s'\n", ret_size, buff);
        }
        close(pipe_fds[0]);
    } else {                        //pid<0为fork失败
        printf("Fail to fork.");
        exit(EXIT_FAILURE);
    }
    printf("Process %d quit.\n", getpid());
    return EXIT_SUCCESS;
}
