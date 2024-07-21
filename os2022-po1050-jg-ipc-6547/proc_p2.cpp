#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

int procP2ErrFD, procP2OutFD, fileFD, pipeWR;

void FatalError();
void sigHandler(int sig);

int main(int argc, char *argv[]){
    
    //open error file
    procP2ErrFD = open("proc_p2.err", O_CREAT | O_RDWR | O_TRUNC, 0777);
    //open out file
    procP2OutFD = open("proc_p2.out", O_CREAT | O_RDWR | O_TRUNC, 0777);
    fileFD = open ("p2.txt", O_RDONLY);

    dprintf(procP2OutFD,"proc_p2.cpp\n");

	if(argc != 2) {
		dprintf(procP2ErrFD, "Chyba: Nespravny pocet argumentov: %d\n", argc - 1);
        errno = EINVAL;
        FatalError();
    }

    signal(SIGUSR1, sigHandler);
    pipeWR = atoi(argv[1]);

    while(1) {
        sleep(1);
    }
	
    close(procP2ErrFD);
    close(procP2OutFD);

}

void FatalError() {
    dprintf(procP2ErrFD, "Koniec. Chyba: %s.\n", strerror(errno));
    exit(EXIT_FAILURE);
}

void sigHandler(int sig) {
dprintf(procP2OutFD, "Prijal som signal SIGUSR1.\n");
dprintf(procP2OutFD, "Nacitavam zo suboru.\n");

char word[150];
char c;
int byteRead;
int pos=0;
while((byteRead = read(fileFD, &c, 1)) > 0 && c != '\n') {
	if (byteRead == -1) {
		dprintf(procP2ErrFD, "Chyba: Nepodarilo sa nacitat zo suboru!\n");
		FatalError();

	}
	word[pos] = c;
	pos++;
}
word[pos] = '\0';
dprintf(procP2OutFD, "Precitane slovo zo suboru: %s\n", word);
word[pos] = '\n';
if (write(pipeWR, word, pos+1) < 0) {
    dprintf(procP2ErrFD, "Chyba: Nepodarilo sa zapisat slovo do rury!\n");
    FatalError();
}
word[pos] = '\0';
dprintf(procP2OutFD, "Zapisal som slovo %s do rury.\n", word);

}
