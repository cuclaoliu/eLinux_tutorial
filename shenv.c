#include <stdio.h>

int main(int argc, char *argv[], char *env[])
{
	while(*env != (char*)NULL)
		printf("%s\n", *env++);
	return 0;
}
