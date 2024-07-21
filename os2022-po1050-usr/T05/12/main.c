#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc, char *argv[], char **envp)
{
    if(argc<2)
    {
        printf("Málo argumentov\n");
        exit(EXIT_FAILURE);
    }
    system(argv[1]);
    return 0;
}