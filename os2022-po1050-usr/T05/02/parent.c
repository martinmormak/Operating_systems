#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 

int main(void)
{
    printf("Pôvodný process ID[%d]\n", getpid());
    printf("Pôvodný parnet process ID[%d]\n", getppid());
    execve("child", NULL, NULL);
    return 0;
}