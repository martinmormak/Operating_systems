#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>
#include <time.h>
#include <dirent.h>

int main(int argc, char* argv[])
{
    DIR *directory;
    struct dirent *stDirent;
    struct stat stStat;
    int ifd;
    struct tm *stTime;
    int dir,fil;
    long dirSize,filSize;
    char path[1000000];
    if( argc < 2 )
    {
        fprintf(stderr, "infile is missing\n");
        exit(EXIT_FAILURE);
    }
    if((directory=opendir(argv[1]))==NULL)
    {
        fprintf(stderr, "%d:%s\n",errno,strerror(errno));
        exit(EXIT_FAILURE);
    }
    dir=0;
    fil=0;
    dirSize=0;
    filSize=0;
    realpath(argv[1], path);
    printf("Directory of %s\n\n",path);
    while ((stDirent=readdir(directory))!=NULL)
    {
        stat(stDirent->d_name,&stStat);
        //if(S_ISDIR(stStat.st_mode)){printf("<DIR>\t");}else{printf("\t");}
        stTime=localtime(&stStat.st_mtime);
        printf("%02d/%02d/%04d\t%02d:%02d\t",stTime->tm_mon,stTime->tm_mday,stTime->tm_year,stTime->tm_hour,stTime->tm_min);
        if(S_ISDIR(stStat.st_mode))
        {
            printf("<DIR>\t\t");
            dir++;
            dirSize=dirSize+stStat.st_size;
        }
        else
        {
            printf("\t%ld\t",stStat.st_size);
            fil++;
            filSize=filSize+stStat.st_size;
        }
        printf("%s\n",stDirent->d_name);
        //printf("\t%ld\t%d\t%d\t%ld\t%02d-%02d-%02d\t%s\n",stStat.st_nlink,stStat.st_uid,stStat.st_gid,stStat.st_size,stTime->tm_mday,stTime->tm_mon+1,stTime->tm_year+1900,stDirent->d_name);
    }
    printf("\t\t\t%d File(s)\t%ld bytes\n\t\t\t%d Dir(s)\t%ld bytes free\n",fil,filSize,dir,dirSize);
    closedir(directory);
    return 0;
}