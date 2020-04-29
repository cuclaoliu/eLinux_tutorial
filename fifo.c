//  实验8：Linux 管道通信
//  fifo.c, 两个进程之间有名管道通信

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <fcntl.h>

#define FIFO "myfifo"
#define BUFFER_SIZE 1024

int main(int argc, char* argv[])
{
    char buff[BUFFER_SIZE];
    int real_write;
    int fds[2];
    
    pid_t pid;
    pipe(fds);
    pid = fork();
    if(pid==(pid_t)0)
    {
        FILE *stream;
        close(fds[1]);
        stream = fdopen(fds[0], "r");
        reader(stream);
        close(fds[0]);
    }
    else
    {
        FILE *stream;
        close(fds[0]);
        stream = fdopen(fds[1], "w");
        writer("Hello, world.", 5, stream);
        close(fds[1]);
    }
    return EXIT_SUCCESS;
}