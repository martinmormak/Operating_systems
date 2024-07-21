#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
int main(int argc, char *argv[])
{
    int status;
    int fd;
    if(argc<2)
    {
        printf("Málo argumentov\n");
        exit(EXIT_FAILURE);
    }
    if((fd=open(argv[1],O_RDWR))<0)
    {
        printf("Chyba pri otvarani suboru\n");
        exit(EXIT_FAILURE);
    }
    if (fork() == 0) {
        dprintf(fd,"child: ahoj ja som dcersky proces ktorý píše do súboru\n");
        dprintf(fd,"child: ahoj ja som dcersky proces ktorý píše do súboru2\n");
        dprintf(fd,"child: ahoj ja som dcersky proces ktorý píše do súboru3\n");
        exit(EXIT_SUCCESS);
    }
    dprintf(fd,"PARENT: AHOJ JA SOM RODICOVSKY PROCES KTORÝ PÍŠE DO SÚBORU\n");
    dprintf(fd,"PARENT: AHOJ JA SOM RODICOVSKY PROCES KTORÝ PÍŠE DO SÚBORU2\n");
    dprintf(fd,"PARENT: AHOJ JA SOM RODICOVSKY PROCES KTORÝ PÍŠE DO SÚBORU3\n");
    wait(&status);
    close(fd);
    return 0;
}