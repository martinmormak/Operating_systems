#include <stdio.h>
#include <unistd.h>
int main (void)
{
    printf("Nový process ID[%d]\n", getpid());
    printf("Nový parnet process ID[%d]\n", getppid());
    return 0;
}