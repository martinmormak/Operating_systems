#include <stdio.h>
int main(int argc, char **argv, char **envp)
{
    printf("\n\nHodnoty argv v child:\n");
    for (char **arg = argv; *arg != 0; arg++)
    {
        char *thisArg = *arg;
        printf("%s\n", thisArg);    
    }
    printf("\n\nHodnoty envp v child:\n");
    for (char **env = envp; *env != 0; env++)
    {
        char *thisEnv = *env;
        printf("%s\n", thisEnv);    
    }
    return 0;
}