#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h> 

int main(int argc, char* argv[])
{
    /*char name[100];*/
    int /*ifd,*/n;
    char buf[10000];
    /*printf("Enter name of input file: ");
    scanf("%s",name);
    if((ifd=open(name,O_RDONLY))<0)
    {
        fprintf(stderr, "%d:%s\n",errno,strerror(errno));
        exit(EXIT_FAILURE);
    }*/
    while((n=read(0,&buf,1000))>0)
    {
        write(1,&buf,n);
    }
    fprintf(stderr, "%d:%s\n",errno,strerror(errno));
    //close(ifd);
    return 0;
}