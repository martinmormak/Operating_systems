#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int number=4;

int main(void)
{
    number = 1;

    if (fork() == 0) {
        printf("%d\n", number);
        exit(EXIT_SUCCESS);
    } else {
        wait(NULL);
    }
    return 0;
}