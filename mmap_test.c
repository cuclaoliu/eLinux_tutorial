#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <time.h>

#define MAX (65536*64)
#define FILE_NAME "mmap_test_file"

int main(){
    int i = 0;
    int count, fd = 0;
    //struct timeval tv1, tv2;
    clock_t start, end;
    int *array = (int *)malloc(sizeof(int) * MAX);

    //生成随机数
    srand((unsigned)time(NULL));
    for (i=0; i < MAX; ++i){
        array[i] = rand();
    }
    //生成需要的文件
    fd = open(FILE_NAME, O_WRONLY | O_CREAT, 0644);
    if(fd == -1){
        perror("create file");
        return EXIT_FAILURE;
    }
    write(fd, array, sizeof(int) * MAX);
    close(fd);

    /*read*/
    //gettimeofday(&tv1, NULL);       //计时开始
    start = clock();
    fd = open(FILE_NAME, O_RDWR);
    if(fd == -1){
        perror("open file for read");
        return EXIT_FAILURE;
    }
    count = read(fd, (void *)array, sizeof(int) * MAX);
    if (sizeof(int) * MAX != count){
        printf("Reading data failed.\n");
        return EXIT_FAILURE;
    }
    for (i = 0; i < MAX; ++i)
        ++array[i];
    if (sizeof(int) * MAX != write(fd, (void *)array, sizeof(int) * MAX)) {
        printf("Writing data failed.\n");
        return EXIT_FAILURE;
    }
    end = clock();
    //gettimeofday(&tv2, NULL);       //计时结束
    //printf("Time of read/write: %d ms.\n", 1000*(tv2.tv_sec-tv1.tv_sec)+(tv2.tv_usec - tv1.tv_usec)/1000);
    printf("Time of read/write: %.3f ms.\n", (double)(end-start)*1e3/CLOCKS_PER_SEC);
    free(array);
    array = NULL;
    close(fd);

    /*mmap*/
    //gettimeofday(&tv1, NULL);
    start = clock();
    fd = open(FILE_NAME, O_RDWR, 0644);
    if(fd == -1){
        perror("open file for mmap");
        return EXIT_FAILURE;
    }
    array = (int*)mmap(NULL, sizeof(int) * MAX, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    for (i = 0; i < MAX; ++i)
        ++array[i];
    munmap(array, sizeof(int) * MAX);
    msync(array, sizeof(int) * MAX, MS_SYNC);
    //free(array);
    //gettimeofday(&tv2, NULL);
    end = clock();
    //printf("Time of mmap: %d ms.\n", 1000*(tv2.tv_sec-tv1.tv_sec)+(tv2.tv_usec - tv1.tv_usec)/1000);
    printf("Time of mmap: %.3f ms.\n", (double)(end-start)*1e3/CLOCKS_PER_SEC);
    close(fd);
    remove(FILE_NAME);
    return 0;
}
