#include <stdio.h>
#include <unistd.h>
int main (void)
{
    printf("Nový getpid[%d]\n", getpid());
    printf("Nový getppid[%d]\n", getppid());
    return 0;
}