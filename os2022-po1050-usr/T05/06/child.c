#include <stdio.h>
int main(int argc, char **argv, char **envp)
{
    printf("\n\nHodnoty argv v child:\n");
    for (char **arg = argv; *arg != 0; arg++)
    {
        char *thisArg = *arg;
        printf("%s\n", thisArg);    
    }
    return 0;
}