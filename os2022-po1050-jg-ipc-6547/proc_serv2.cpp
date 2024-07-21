#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netdb.h>

int procServ2OutFD, procServ2ErrFD;

void FatalError();

int main(int argc, char *argv[]){
    
    //open error file
    procServ2ErrFD = open("proc_serv2.err", O_CREAT | O_RDWR | O_TRUNC, 0777);
    //open out file
    procServ2OutFD = open("proc_serv2.out", O_CREAT | O_RDWR | O_TRUNC, 0777);

    if(argc!=2){
        dprintf(procServ2ErrFD,"Chyba: Nespravny pocet argumetov ocakavam 1 dostal som %d\n",argc-1);
        errno = EINVAL;
        FatalError();
    }
    
    int port = atoi(argv[1]);
    
    //crete socket
    dprintf(procServ2OutFD,"Vytvaram socket..\n");
    int socketFD=socket(AF_INET, SOCK_DGRAM,0);
    if(socketFD<0)
    {
        dprintf(procServ2ErrFD,"Chyba: Nevytvoril som socket.\n");
        FatalError();
    }
    dprintf(procServ2OutFD,"Vytvoril som socket.\n");

    //find server
    
    dprintf(procServ2OutFD, "Hladanie serveru s adresou 127.0.0.1 .\n");
    struct hostent *serv = gethostbyname("127.0.0.1");
    if (serv == NULL){
        dprintf(procServ2ErrFD, "Chyba: Nenasiel sa server na adrese 127.0.0.1 .\n");
        FatalError();
    }
    dprintf(procServ2OutFD, "Server najdeny.\n");

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    bcopy((char *)serv->h_addr, (char *)&serv_addr.sin_addr.s_addr, serv->h_length);

    dprintf(procServ2OutFD, "Pripojovanie na server...\n");
    if (bind(socketFD, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
        dprintf(procServ2ErrFD, "Chyba: Zlyhalo pripojenie na server.\n");
        FatalError();
    }
    dprintf(procServ2OutFD, "Pripojenie na server bolo uspesne.\n");

    int outputFileFd = open("serv2.txt", O_CREAT | O_RDWR | O_TRUNC, 0777);
    
    char word[301];
    do{
        struct sockaddr_in serv2;
        socklen_t serv2size = sizeof(struct sockaddr_in);
        dprintf(procServ2OutFD,"Citanie zo servera...\n");
        bzero(word, sizeof(word));

        int readChar = recvfrom(socketFD, word, sizeof(word), 0, (struct sockaddr*)&serv2, &serv2size);
        if(readChar < 0){
            dprintf(procServ2ErrFD, "Chyba: Neprecital som zo servera.\n");
            FatalError();
        }
        word[readChar]='\0';
        dprintf(procServ2OutFD,"Precital som slovo: %s\n",word);
        dprintf(procServ2OutFD,"Zapisujem slovo do suboru.\n");
        word[readChar]='\n';
        if(write(outputFileFd,word,strlen(word))<0){
            dprintf(procServ2ErrFD,"Chyba: Nepodarilo sa zapisat slovo do suboru.\n");
            FatalError();
        }
        write(outputFileFd,"\n",1);
        dprintf(procServ2OutFD,"Slovo som uspecne zapisal\n");
    }while(true);

    close(procServ2ErrFD);
    close(procServ2OutFD);
}

void FatalError() {
    dprintf(procServ2ErrFD, "Koniec. Chyba: %s.\n", strerror(errno));
    exit(EXIT_FAILURE);
}