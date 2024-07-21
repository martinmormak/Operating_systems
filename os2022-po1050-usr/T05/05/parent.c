#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 

int main(void)
{
    printf("Pôvodný getpid[%d]\n", getpid());
    printf("Pôvodný getppid[%d]\n", getppid());
    if (fork() == 0)
    {
        execve("child", NULL, NULL);
        exit(0);
    }
    return 0;
}