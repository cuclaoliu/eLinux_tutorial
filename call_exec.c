#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
//编译命令：gcc call_exec.c -o call_exec -lreadline

int main(int argc, char *argv[]){
	char *input;
	pid_t pid;
	while (1)	{
		char *parsed;
		char *command[8] = {NULL};
		int index = 0;
		input = readline("MyShell> "); //在提示符下输入命令
		parsed = strtok(input, " ");   //分解命令
		while (parsed != NULL && index < 8)	{
			command[index++] = parsed; //分解出命令的各个参数
			parsed = strtok(NULL, " ");
		}
		if (strncmp(command[0], "bye", 3) == 0)
			break;	//exit命令退出
		pid = fork(); //保持当前进程，fork子进程去执行用户命令
		if (pid == 0)		{
			if (execvp(command[0], command) < 0){//执行不带路径的命令
				printf("Bad command!\n"); //执行命令失败才会到这里，退出子进程
				perror(command[0]);
				return EXIT_FAILURE;
			}
			printf("Should NOT be here!\n"); //执行成功是不应该到这里的
		} else if (pid > 0)		{
			wait(NULL); //等待所有子进程退出
			//usleep(10000);
		} else {
			printf("fork failed!\n");
			return EXIT_FAILURE;
		}
	}
	return EXIT_SUCCESS;
}