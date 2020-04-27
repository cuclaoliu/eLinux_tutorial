//  实验8：Linux 管道通信
//  pipe_SIFPIPE.c 两个进程之间无名管道通信，验证写端对读端存在的依懒性

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024
#define DELAY_TIME 1

int main(int argc, char** argv)
{
    int pipe_fds[2];
    pid_t pid;
    int ret_size;
    char buff[BUFFER_SIZE];
    const char data[] = "pipe test 123456";

    memset((void*)buff, 0, sizeof(buff));
    if(pipe(pipe_fds)<0)
    {
        printf("pipe create failed\n");
        return EXIT_FAILURE;
    }
    if((pid = fork())==(pid_t)0)
    {                                       //子进程
        close(pipe_fds[1]);                 //关闭管道写
        while(1)
        {
            sleep(DELAY_TIME);
            if((ret_size=read(pipe_fds[0], buff, BUFFER_SIZE))>0)
            {
               printf("%d bytes readed from the pipe is: '%s'\n", ret_size, buff);
            }
            else
            {
               printf("Read pipe Failed.\n");
               return EXIT_FAILURE;
            }
        }
        close(pipe_fds[0]);
        return 0;
    }
    else if(pid > 0)
    {                                       //父进程
        close(pipe_fds[0]);
        sleep(DELAY_TIME);                //等待子进程关闭文件写端
        if((ret_size=write(pipe_fds[1], data, strlen(data)))!=-1)
        {
            printf("%d bytes wrote into the pipe is: '%s'\n", ret_size, data);
        }
        else
        {
            printf("write pipe FAILED!\n");
        }
        ret_size = write(pipe_fds[1], data, 4096);
        close(pipe_fds[1]);
    }
    else
    {
        printf("create child process failed.");
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}