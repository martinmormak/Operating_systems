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
    int ifd;
    long n;
    char buf[1000000];
    if( argc < 5 )
    {
        fprintf(stderr, "infile, char count, position or char is missing\n");
        exit(EXIT_FAILURE);
    }
    if((ifd=open(argv[1],O_RDWR))<0)
    {
        fprintf(stderr, "%d:%s\n",errno,strerror(errno));
        exit(EXIT_FAILURE);
    }
    if((n=lseek(ifd, atoll(argv[3]), SEEK_SET ))<0)
    {
        fprintf(stderr, "%d:%s\n",errno,strerror(errno));
        exit(EXIT_FAILURE);
    }
    for(int i=0;i<atoi(argv[2]);i++)
    {
        write(ifd,argv[4],1);
    }
    fprintf(stderr, "%d:%s\n",errno,strerror(errno));
    close(ifd);
    return 0;
}