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
    struct stat stStat;
    struct tm *stTime;
    
    if( argc < 2 )
    {
        fprintf(stderr, "infile is missing\n");
        exit(EXIT_FAILURE);
    }
    if((ifd=open(argv[1],O_RDONLY))<0)
    {
        fprintf(stderr, "%d:%s\n",errno,strerror(errno));
        exit(EXIT_FAILURE);
    }
    fstat(ifd,&stStat);
    printf("%ld\n",stStat.st_ino);
    close(ifd);
    return 0;
}