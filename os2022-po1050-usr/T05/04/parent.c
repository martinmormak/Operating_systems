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
    printf("\n\nHodnoty envp v parent:\n");
    for (char **env = envp; *env != 0; env++)
    {
        char *thisEnv = *env;
        printf("%s\n", thisEnv);    
    }
    execve("child", argv, envp);
    return 0;
}