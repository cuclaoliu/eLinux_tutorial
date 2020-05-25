/***  drvdemo_read.c   ***/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#define MAX_LEN 32
const char* DRVNAME = "drvdemo";

int main(){
    int fd;
    int i;
    char buf[256];
    fd = open(DRVNAME, O_RDONLY);
    if (fd < 0)    {
        printf("open %s failed.\n", DRVNAME);
        return EXIT_FAILURE;
    }
    i = read(fd, buf, MAX_LEN);
    printf("Read %d bytes data from %s \n", i, DRVNAME);
    buf[i] = '\0';
    printf("Content read from %s:\n%s\n", DRVNAME, buf);
    //ioctl(fd, 1, NULL);
    //ioctl(fd, 4, NULL);
    close(fd);
    return 0;
}
