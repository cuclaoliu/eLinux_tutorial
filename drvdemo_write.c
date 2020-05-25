/***  drvdemo_write.c   ***/
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <string.h>
#define MAX_LEN 32
const char* DRVNAME = "drvdemo";

int main(int argc, char** argv){
    int fd;
    int i;
    if (argc < 2){
        printf("usage: drvdemo_write string_to_write\n");
        return EXIT_FAILURE;
    }
    fd = open(DRVNAME, O_WRONLY);
    if (fd < 0)    {
        printf("open %s failed.\n", DRVNAME);
        return EXIT_FAILURE;
    }
    i = write(fd, argv[1], strlen(argv[1]));
    printf("write %d bytes data to %s:\n%s\n", i, DRVNAME, argv[1]);
    //ioctl(fd, 1, NULL);
    //ioctl(fd, 4, NULL);
    close(fd);
    return 0;
}
