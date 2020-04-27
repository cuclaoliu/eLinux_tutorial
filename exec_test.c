#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	if (argc < 2)
		return EXIT_FAILURE;
	for(int i=1; i<argc; i++)
		printf("%s ", argv[i]);
       printf("\n");	
	execvp(argv[1], argv+1);
	printf("After execvp\n"); //执行命令失败才会到这里，退出子进程
	return EXIT_SUCCESS;
}
