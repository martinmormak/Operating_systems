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
    int c=0,k=1;
    char rv;
    struct termio initSet, newSet;
    ioctl(0,TCGETA,&initSet);
    newSet=initSet;
    newSet.c_lflag=~ICANON & ~ECHO;
    newSet.c_cc[VTIME]=0;
    newSet.c_cc[VMIN]=0;
    ioctl(0,TCSETA,&newSet);
    while(1)
    {
        printf("%d\n",c);
        usleep((useconds_t) 500000);
        c = c + k;
        read(0,&rv,1);
        if(rv<='9' && rv >='0' )
        {
            k = rv - 0x30;
        }
    }
    ioctl(0,TCSETA,&initSet);
    return 0;
}