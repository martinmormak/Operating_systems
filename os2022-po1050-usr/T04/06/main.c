#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <termio.h>
#define LOGIN_LEN 8
#define PASSWORD_LEN 8
int main()
{
    char c;
    struct termio initSet, newSet;
    ioctl(0,TCGETA,&initSet);
    newSet=initSet;
    newSet.c_lflag=~ICANON & ~ECHO;
    newSet.c_cc[VTIME]=5;
    newSet.c_cc[VMIN]=0;
    ioctl(0,TCSETA,&newSet);
    while(1)
    {
        read(0,&c,1);
        write(1,&c,1);
    }
    ioctl(0,TCSETA,&initSet);
    return 0;
}