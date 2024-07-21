#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/sem.h>
#include <sys/shm.h>

int procTOutFD, procTErrFD;

void FatalError();

int main(int argc, char *argv[]){
    
    //open error file
    procTErrFD = open("proc_t.err", O_CREAT | O_RDWR | O_TRUNC, 0777);
    //open out file
    procTOutFD = open("proc_t.out", O_CREAT | O_RDWR | O_TRUNC, 0777);

    if(argc!=4){
        dprintf(procTErrFD,"Chyba: Nespravny pocet argumetov ocakavam 3 dostal som %d\n",argc-1);
        errno = EINVAL;
        FatalError();
    }

    int pipeRead=atoi(argv[1]);
    int shm=atoi(argv[2]);
    int sem=atoi(argv[3]);
    
    //shared memory
    dprintf(procTOutFD,"Pripajam sa na zdielanu pamat (ID: %d).\n",shm);
    char* shm_ptr = (char*) shmat(shm,NULL,0);
    if(shm_ptr == (void *)-1){
        dprintf(procTErrFD,"Chyba: Zlyhalo pripojenie na zdielanu pamat.");
        FatalError();
    }
    dprintf(procTOutFD,"Pripojenie k zdielanaj pamati sa podarilo.\n");

    //read and writ loop
    char word[301];
    do
    {
        int readChar,wordPosition=0;
        char c;
        dprintf(procTOutFD,"Citanie z pipy.\n");
        while((readChar=read(pipeRead,&c,1))!=0&&c!='\n')
        {
            if(readChar==-1){
                dprintf(procTErrFD,"Chyba: neprecital som znak.\n");
                FatalError();
            }
            word[wordPosition] = c;
            wordPosition++;
            word[wordPosition] = '\0';
        }
        dprintf(procTOutFD,"Precital som slovo: %s\n",word);
        dprintf(procTOutFD,"Zapisujem slovo do zdielanej pamate.\n");
        if(sprintf(shm_ptr,"%s",word)<0)
        {
            dprintf(procTErrFD,"Chyba: Nezapisal som slovo do zdielanej pamate.\n");
            FatalError();
        }
        dprintf(procTOutFD,"Zapisal som slovo do zdielanej pamate.\n");

        //sem[1]
        struct sembuf s;
        s.sem_num = 1;
        s.sem_op = 1;
        s.sem_flg = 0;
        
        if (semop(sem, &s, 1) < 0) {
            dprintf(procTErrFD, "Chyba: Nastavenie semafora S1[0] zlyhalo.\n");
            FatalError();
        }
        dprintf(procTOutFD, "Semafor S1[0] je nastaveny na zelenu.\n");
        
        //sem[0]
        dprintf(procTOutFD, "Nastavenie cervenej na semafore S1[1]...\n");
        s.sem_num = 0;
        s.sem_op = -1;
        s.sem_flg = 0;
        if (semop(sem, &s, 1) < 0) {
            dprintf(procTErrFD, "Chyba: Nastavenie semafora S1[1] zlyhalo.\n");
            FatalError();
        }
        dprintf(procTOutFD, "Semafor S1[1] je nastaveny na cervenu.\n");
    }while(true);

    dprintf(procTOutFD,"proc_t.cpp\n");

    close(procTErrFD);
    close(procTOutFD);
}

void FatalError() {
    dprintf(procTErrFD, "Koniec. Chyba: %s.\n", strerror(errno));
    exit(EXIT_FAILURE);
}
