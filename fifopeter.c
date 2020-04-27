/***   fifopeter.c  ***/
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

int main(void) {
  char write_fifo_name[] = "Peter"; /*Peter喊话FIFO*/
  char read_fifo_name[] = "Lucy";   /*Lucy喊话FIFO*/
  int write_fd, read_fd;
  char buf[256];
  int len;
  if (mkfifo(write_fifo_name, S_IRUSR | S_IWUSR) == -1){
    printf("Fail to create FIFO %s: %s", write_fifo_name, strerror(errno));
    exit(EXIT_FAILURE);
  }
  printf("Waiting for %s...\n", read_fifo_name);
  if ((read_fd = open(read_fifo_name, O_RDONLY)) == -1) {
    printf("Fail to open FIFO %s: %s", read_fifo_name, strerror(errno));
    exit(EXIT_FAILURE);
  }
  if ((write_fd = open(write_fifo_name, O_WRONLY)) == -1) {
    printf("Fail to open FIFO %s: %s", write_fifo_name, strerror(errno));
    exit(EXIT_FAILURE);
  }
  printf("Here is %s.\n", read_fifo_name);
  while (1) {
    printf("Waiting for %s's message...\n", read_fifo_name);
    if ((len = read(read_fd, buf, 256)) > 0) {
      buf[len] = '\0';
      printf("%s: %s\n", read_fifo_name, buf);
      if (strncmp(buf, "bye", 4) == 0) {
        break;
      }
    }
    printf("%s: ", write_fifo_name);
    fgets(buf, 256, stdin);
    buf[strlen(buf) - 1] = '\0';
    write(write_fd, buf, strlen(buf));
    if (strncmp(buf, "bye", 4) == 0) {
    sleep(1);
      break;
    }
  }
  close(write_fd);
  unlink(write_fifo_name);
  close(read_fd);
  return EXIT_SUCCESS;
}
