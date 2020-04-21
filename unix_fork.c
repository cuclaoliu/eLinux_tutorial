#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t pid;
    int status;
    /* fork a child process */
    pid = fork();
    if (pid < 0) { /* error occurred */
        fprintf(stderr, "Fork Failed");
        return EXIT_FAILURE;
    }
    else if (pid == 0) { /* child process */
        execlp("/bin/ls","ls",NULL);
        sleep(3);
    }
    else { /* parent process */
        /* parent will wait for the child to complete */
        waitpid(pid, &status, WNOHANG);
        printf("Child Complete\n");
    }
    return EXIT_SUCCESS;
}