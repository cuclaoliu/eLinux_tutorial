/***  test_demo.c   ***/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#define MAX_LEN 32
const char* DRVNAME = "/dev/drvdemo";
void showbuf(char *buf);

int main(){
    int fd;
    int i;
    char buf[255];
    for (i = 0; i < MAX_LEN; i++)    {
        buf[i] = i;
    }
    fd = open(DRVNAME, O_RDWR);
    if (fd < 0)    {
        printf("open %s failed.\n", DRVNAME);
        return EXIT_FAILURE;
    }
    printf("write %d bytes data to %s \n", MAX_LEN, DRVNAME);
    showbuf(buf);
    write(fd, buf, MAX_LEN);
    printf("Read %d bytes data from %s \n", MAX_LEN, DRVNAME);
    read(fd, buf, MAX_LEN);
    showbuf(buf);
    ioctl(fd, 1, NULL);
    ioctl(fd, 4, NULL);
    close(fd);
    return 0;
}
void showbuf(char *buf){
    int i, j = 0;
    for (i = 0; i < MAX_LEN; i++)    {
        if (i % 4 == 0)
            printf("\n%4d: ", j++);
        printf("%4d ", buf[i]);
    }
    printf("\n*****************************************************\n");
}
