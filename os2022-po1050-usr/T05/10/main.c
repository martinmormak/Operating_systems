#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
int main(void)
{
    char cwd[1000];
    int status;
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Aktuálny priečinok: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }
    if (fork() == 0) {
        chdir("..");
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Priečinok po zmene v child: %s\n", cwd);
        } else {
            perror("getcwd() error");
            return 1;
        }
        exit(EXIT_SUCCESS);
    } else {
        wait(&status);
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("Priečinok po zmene v parent: %s\n", cwd);
        } else {
            perror("getcwd() error");
            return 1;
        }
    }
    return 0;
}