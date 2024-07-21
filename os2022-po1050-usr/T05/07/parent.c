#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h> 

int main(int argc, char **argv, char **envp)
{
    printf("Hodnoty envp v parent:\n");
    for (char **env = envp; *env != 0; env++)
    {
        char *thisEnv = *env;
        printf("%s\n", thisEnv);    
    }
    if (fork() == 0)
    {
        execve("child", argv, envp);
        exit(0);
    }
    return 0;
}