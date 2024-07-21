#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <sys/ipc.h>
#include <sys/wait.h>

int procDOutFD, procDErrFD;

void FatalError();

int main(int argc, char *argv[]){
    procDOutFD = open("proc_d.out", O_CREAT | O_RDWR | O_TRUNC, 0777);
    procDErrFD = open("proc_d.err", O_CREAT | O_RDWR | O_TRUNC, 0777);

 if (argc != 4){
    dprintf(procDErrFD, "Chyba: Nespravny pocet argumentov: %d!\n", argc - 1);
    errno = EINVAL;
    FatalError();
}

int shm = atoi(argv[1]);
int sem = atoi(argv[2]);
int port = atoi(argv[3]);

//vytvorenie socketu
dprintf(procDOutFD, "Vytvaranie socketu...\n");
int socketFD = socket(AF_INET, SOCK_STREAM, 0); 
if (socketFD == -1) {
    dprintf(procDErrFD, "Chyba: Tvorba socketu zlyhala.\n");
    FatalError();
}
dprintf(procDOutFD, "Vytvoril som socket.\n");

//find server
dprintf(procDOutFD, "Hladanie serveru s adresou 127.0.0.1 .\n");
struct hostent *serv = gethostbyname("127.0.0.1");
if (serv == NULL){
    dprintf(procDErrFD, "Chyba: Nenasiel sa server na adrese 127.0.0.1 .\n");
    FatalError();
}
dprintf(procDOutFD, "Server najdeny.\n");

struct sockaddr_in serv_addr;
serv_addr.sin_family = AF_INET;
serv_addr.sin_port = htons(port);
bcopy((char *)serv->h_addr, (char *)&serv_addr.sin_addr.s_addr, serv->h_length);

dprintf(procDOutFD, "Pripojovanie na server...\n");
if (connect(socketFD, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
    dprintf(procDErrFD, "Chyba:Zlyhalo pripojenie na server.\n");
    FatalError();
}
dprintf(procDOutFD, "Pripojenie na server bolo uspesne.\n");

//shared memory
dprintf(procDOutFD, "Pripajanie na zdielanu pamat (ID: %d)\n", shm);
char * shm_ptr = (char *) shmat(shm, NULL, 0);
if (shm_ptr == (void *)-1){
    dprintf(procDErrFD, "Chyba: Zlyhalo pripojenie na zdielanu pamat.\n");
    FatalError();
}
dprintf(procDOutFD, "Pripojenie na zdielanu pamat bolo uspesne.\n");


char word[300];

do {
bzero(word, sizeof(word));

dprintf(procDOutFD, "Citanie zo zdielanej pamate...\n");
if (sprintf(word, "%s", shm_ptr) < 0) {
    dprintf(procDErrFD, "Chyba: Zlyhalo citanie zo zdielanej pamate.\n");
    FatalError();
}
dprintf(procDOutFD, "Precitane slovo zo zdielanej pamate: %s.\n", word);

dprintf(procDOutFD, "Odosielanie slova na server...\n");
if (write(socketFD, word, sizeof(word)) < 0){
    dprintf(procDErrFD, "Chyba: Zlyhalo odoslanie slova.\n");
    FatalError();
}
dprintf(procDOutFD, "Odoslanie slova uspesne.\n");

dprintf(procDOutFD, "Nastavenie zelenej na semafore S2[0]...\n");
struct sembuf s;
s.sem_num = 0;
s.sem_op = 1;
s.sem_flg = 0;

if (semop(sem, &s, 1) < 0) {
    dprintf(procDErrFD, "Nastavenie semafora S2[0] zlyhalo.\n");
    FatalError();
}
dprintf(procDOutFD, "Semafor S2[0] je nastaveny na zelenu.\n");


dprintf(procDOutFD, "Nastavenie cervenej na semafore S2[1]...\n");
s.sem_num = 1;
s.sem_op = -1;
s.sem_flg = 0;
if (semop(sem, &s, 1) < 0) {
    dprintf(procDErrFD, "Nastavenie semafora S2[1] zlyhalo.\n");
    FatalError();
}
dprintf(procDOutFD, "Semafor S2[1] je nastaveny na cervenu.\n");
} while(1);


}


void FatalError() {
    dprintf(procDErrFD, "Koniec. Chyba: %s.\n", strerror(errno));
    exit(EXIT_FAILURE);
}
