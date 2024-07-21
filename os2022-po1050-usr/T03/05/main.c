#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <time.h>

int main(int argc, char* argv[])
{
    int ifd;
    
    if( argc < 2 )
    {
        fprintf(stderr, "infile is missing\n");
        exit(EXIT_FAILURE);
    }
    /*if(link(argv[1],"outfile")<0)
    {
        fprintf(stderr, "error to make ln\n");
        exit(EXIT_FAILURE);
    }*/
    if((ifd=open(argv[1],O_RDWR))<0)
    {
        fprintf(stderr, "%d:%s\n",errno,strerror(errno));
        exit(EXIT_FAILURE);
    }
    lseek(ifd,0,SEEK_END);
    write(ifd,"Martin",6);
    close(ifd);
    return 0;
}