#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#define LOGIN_LEN 8
#define PASSWORD_LEN 8
int main()
{
    struct termios initSet, newSet;
    tcgetattr(fileno(stdin),&initSet);
    newSet=initSet;
    newSet.c_cc[VINTR]='c';
    if (tcsetattr(fileno(stdin),TCSAFLUSH,&newSet)<0 ){
        fprintf(stderr,"Could not set attributes \n");
    }
    while(1){
        printf("while\n");
    }
    return 0;
}