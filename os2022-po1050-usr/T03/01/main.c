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
    int sifd;
    if( argc < 2 )
    {
        fprintf(stderr, "infile is missing\n");
        exit(EXIT_FAILURE);
    }
    if((ifd=open(argv[1],O_RDONLY|O_CREAT,0644))<0)
    {
        fprintf(stderr, "%d:%s\n",errno,strerror(errno));
        exit(EXIT_FAILURE);
    }
    lseek(ifd,10,SEEK_SET);
    sifd=dup(ifd);
    printf("%ld\n",lseek(sifd,0,SEEK_CUR));
    close(ifd);
    close(sifd);
    return 0;
}