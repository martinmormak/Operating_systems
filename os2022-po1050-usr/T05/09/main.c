#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

static int *number;

int main(void)
{
    number = mmap(NULL, sizeof *number, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);

    *number = 1;

    if (fork() == 0) {
        *number = 4;
        exit(EXIT_SUCCESS);
    } else {
        wait(NULL);
        printf("%d\n", *number);
        munmap(number, sizeof *number);
    }
    return 0;
}