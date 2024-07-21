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
    if(link(argv[1],argv[2])<0)
    {
        if(link(argv[2],argv[1])<0)
        {
            fprintf(stderr, "error to make ln\n");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}