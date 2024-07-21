#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 

int main(int argc, char **argv, char **envp)
{
    printf("Hodnoty argv v parent:\n");
    for (char **arg = argv; *arg != 0; arg++)
    {
        char *thisArg = *arg;
        printf("%s\n", thisArg);    
    }
    if (fork() == 0)
    {
        execve("child", argv, envp);
        exit(0);
    }
    return 0;
}