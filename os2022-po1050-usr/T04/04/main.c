#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#define LOGIN_LEN 8
#define PASSWORD_LEN 8
int main()
{
    struct termios initSet, newSet;
    char login[LOGIN_LEN+1], password[PASSWORD_LEN+1];
    printf("Enter login:\t");
    scanf("%s",login);
    tcgetattr(fileno(stdin),&initSet);
    newSet=initSet;
    newSet.c_lflag&=~ECHO;
    printf("Enter password: ");
    if (tcsetattr(fileno(stdin),TCSAFLUSH,&newSet)<0 ){
        fprintf(stderr,"Could not set attributes \n");
    }else {
        scanf("%s",password);
        tcsetattr(fileno(stdin),TCSANOW,&initSet);
        printf("\nYour login is %s",login);
        printf("\nYour password is %s\n",password);
    }
    return 0;
} 