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
    if(S_ISDIR(stStat.st_mode)){printf("d");}else{printf("-");}
    if(stStat.st_mode & S_IRUSR){printf("r");}else{printf("-");}
    if(stStat.st_mode & S_IWUSR){printf("w");}else{printf("-");}
    if(stStat.st_mode & S_IXUSR){printf("x");}else{printf("-");}
    if(stStat.st_mode & S_IRGRP){printf("r");}else{printf("-");}
    if(stStat.st_mode & S_IWGRP){printf("w");}else{printf("-");}
    if(stStat.st_mode & S_IXGRP){printf("x");}else{printf("-");}
    if(stStat.st_mode & S_IROTH){printf("r");}else{printf("-");}
    if(stStat.st_mode & S_IWOTH){printf("w");}else{printf("-");}
    if(stStat.st_mode & S_IXOTH){printf("x");}else{printf("-");}
    stTime=localtime(&stStat.st_mtime);
    printf("\t%ld\t%d\t%d\t%ld\t%d\t%02d\t%d:%d\t%s\n",stStat.st_nlink,stStat.st_uid,stStat.st_gid,stStat.st_size,stTime->tm_mon+1,stTime->tm_mday,stTime->tm_hour,stTime->tm_min,argv[1]);
    close(ifd);
    return 0;
}