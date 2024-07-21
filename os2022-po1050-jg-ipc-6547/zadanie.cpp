#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define LENGHT 100
#define PAUSE 1

void FatalError();
void handleSignal(int signum);
int wordCounter();
void createPipe(int pipeArray[2],int order);
int createSHM(int size,int order);
int createSEM(int order);
void procP1(char pipeR1Write[LENGHT]);
void procP2(char pipeR1Write[LENGHT]);
void procPr(char p1PID[LENGHT],char p2PID[LENGHT],char pipeR1Read[LENGHT],char pipeR2Write[LENGHT]);
void procT(char pipeR2Read[], char SM1[], char S1[]);
void procS(char shmSM1[], char S1[], char SM2[], char S2[]);
void procD(char shmSM2[], char S2[], char serverPort1[]);
void procServ1(char serverPort1[], char serverPort2[]);
void procServ2(char serverPort2[]);
void killAllProcesses();

int zadanieErrFD,zadanieOutFD,waiter,p1PID,p2PID,prPID,tPID,sPID,dPID,serv1PID,serv2PID;

int main(int argc, char *argv[]){

    signal(SIGUSR1, handleSignal);

    //open error file
    zadanieErrFD = open("zadanie.err", O_CREAT | O_RDWR | O_TRUNC, 0777);
    //open out file
    zadanieOutFD = open("zadanie.out", O_CREAT | O_RDWR | O_TRUNC, 0777);

    if(argc!=3){
        dprintf(zadanieErrFD,"Chyba: Nespravny pocet argumetov ocakavam 2 dostal som %d\n",argc-1);
        errno = EINVAL;
        FatalError();
    }

    char serverPort1[LENGHT];
    sprintf(serverPort1, "%s", argv[1]);
    char serverPort2[LENGHT];
    sprintf(serverPort2, "%s", argv[2]);
    
    //create R1 pipe
    int pipeR1FD[2];
    createPipe(pipeR1FD,1);
    char pipeR1Read[LENGHT];
    sprintf(pipeR1Read,"%d",pipeR1FD[0]);
    pipeR1Read[strlen(pipeR1Read)] = '\0';
    char pipeR1Write[LENGHT];
    sprintf(pipeR1Write,"%d",pipeR1FD[1]);
    pipeR1Write[strlen(pipeR1Write)] = '\0';
    sleep(PAUSE);
    
    //create R2 pipe
    int pipeR2FD[2];
    createPipe(pipeR2FD,2);
    char pipeR2Read[LENGHT];
    sprintf(pipeR2Read,"%d",pipeR2FD[0]);
    pipeR2Read[strlen(pipeR2Read)] = '\0';
    char pipeR2Write[LENGHT];
    sprintf(pipeR2Write,"%d",pipeR2FD[1]);
    pipeR2Write[strlen(pipeR2Write)] = '\0';
    sleep(PAUSE);

    //create share memory
    int numberOfWords = wordCounter();
    //share memory 1
    int shmd1;
    shmd1=createSHM(numberOfWords,1);
    char SM1[LENGHT];
    sprintf(SM1,"%d",shmd1);
    SM1[strlen(SM1)] = '\0';
    sleep(PAUSE);
    //share memory 2
    int shmd2;
    shmd2=createSHM(numberOfWords,2);
    char SM2[LENGHT];
    sprintf(SM2,"%d",shmd2);
    SM2[strlen(SM2)] = '\0';
    sleep(PAUSE);

    
    //create semaphores
    //semaphore 1
    int semd1;
    semd1=createSEM(1);
    char S1[LENGHT];
    sprintf(S1,"%d",semd1);
    S1[strlen(S1)] = '\0';
    sleep(PAUSE);
    //semaphore 2
    int semd2;
    semd2=createSEM(1);
    char S2[LENGHT];
    sprintf(S2,"%d",semd2);
    S2[strlen(S2)] = '\0';
    sleep(PAUSE);

    //fork proc_p1
    procP1(pipeR1Write);
    char p1[LENGHT];
    sprintf(p1,"%d",p1PID);
    p1[strlen(p1)] = '\0';
    sleep(PAUSE);

    //fork proc_p2
    procP2(pipeR1Write);
    char p2[LENGHT];
    sprintf(p2,"%d",p2PID);
    p2[strlen(p2)] = '\0';
    sleep(PAUSE);
    
    //fork proc_t
    procT(pipeR2Read,SM1,S1);
    sleep(PAUSE);
    
    //fork proc_s
    waiter = 1;
    procS(SM1,S1,SM2,S2);
    while(waiter == 1){
        sleep(PAUSE);
    }
    
    //fork proc_serv2
    procServ2(serverPort2);
    sleep(PAUSE);
    
    //fork proc_serv1
    waiter = 1;
    procServ1(serverPort1, serverPort2);
    sleep(PAUSE);
    while(waiter == 1){
        sleep(PAUSE);
    }
    
    //fork proc_d
    procD(SM2,S2,serverPort1);
    sleep(PAUSE);
    
    //fork proc_pr
    procPr(p1,p2,pipeR1Read,pipeR2Write);
    sleep(PAUSE);

    int status;
    waitpid(prPID, &status, 0);
    sleep(10);

    killAllProcesses();
    
    close(pipeR1FD[0]);
    close(pipeR1FD[1]);
    close(pipeR2FD[0]);
    close(pipeR2FD[1]);

    semctl(semd1, 0, IPC_RMID, 0);
    semctl(semd2, 0, IPC_RMID, 0);
    shmctl(shmd1, 0, IPC_RMID);
    shmctl(shmd2, 0, IPC_RMID);

    dprintf(zadanieOutFD, "Program sa uspesne ukoncil!\n");

    close(zadanieErrFD);
    close(zadanieOutFD);

    return 0;
}

void FatalError()
{
    dprintf(zadanieErrFD, "Koniec. Chyba: %s\n",strerror(errno));
    exit(EXIT_FAILURE);
}

void handleSignal(int signum){
    dprintf(zadanieOutFD, "Prijal som signal SIGUSR1.\n");
    waiter=0;
}

int wordCounter(){
    int txtFD, countWords=0;
    char c;
    for (int i = 0; i < 2; i++)
    {
        if (i == 0)
            txtFD = open("p1.txt", O_RDONLY);
        else if (i == 1)
            txtFD = open("p2.txt", O_RDONLY);
        if(txtFD < 0){
            dprintf(zadanieErrFD, "Chyba: Subor neexistuje.\n");
            FatalError();
        }
        while (read(txtFD, &c, 1)!= 0)
        {
            if (c == '\n')
                countWords++;
        }
        close(txtFD);
    }
    return countWords;
}

void createPipe(int pipeArray[2],int order){
    dprintf(zadanieOutFD, "Vytvaram novu %d pipe.\n",order);
    if(pipe(pipeArray)<0){
        dprintf(zadanieErrFD,"Chyba: Vytvorenie pipe %d sa nepodarilo.\n",order);
        FatalError();
    }
    dprintf(zadanieOutFD,"Vytvorenie pipy %d sa podarilo.\n",order);
}

int createSHM(int size,int order){
    int shmd;
    dprintf(zadanieOutFD, "Vytvaram novu %d zdielanu pamat.\n",order);
    if((shmd=shmget(IPC_PRIVATE, size*151, 0666 | IPC_CREAT))<0){
        dprintf(zadanieErrFD,"Chyba: Vytvorenie zdielanej pamate %d sa nepodarilo.\n",order);
        FatalError();
    }
    dprintf(zadanieOutFD,"Vytvorenie zdielanej pamate %d sa podarilo.\n",order);
    return shmd;
}


int createSEM(int order){
    int semd;
    dprintf(zadanieOutFD, "Vytvaram novy %d semafor.\n",order);
    if((semd=semget(IPC_PRIVATE, 2, 0666 | IPC_CREAT))<0){
        dprintf(zadanieErrFD,"Chyba: Vytvorenie semaforu %d sa nepodarilo.\n",order);
        FatalError();
    }
    dprintf(zadanieOutFD,"Vytvorenie semaforu %d sa podarilo.\n",order);
    /*struct sembuf s;
    s.sem_num = 0;
    s.sem_op = 1;
    s.sem_flg = 0;
    if (semop(semd, &s, 1) < 0) {
        dprintf(zadanieErrFD, "Nastavenie semafora S[0] zlyhalo.\n");
        FatalError();
    }
    dprintf(zadanieOutFD, "Semafor S[0] je nastaveny na zelenu.\n");
    s.sem_num = 1;
    s.sem_op = 0;
    s.sem_flg = 0;
    if (semop(semd, &s, 1) < 0) {
        dprintf(zadanieErrFD, "Nastavenie semafora S[1] zlyhalo.\n");
        FatalError();
    }
    dprintf(zadanieOutFD, "Semafor S[1] je nastaveny na cervenu.\n");*/
    return semd;
}

void procP1(char pipeR1Write[LENGHT]){
    dprintf(zadanieOutFD, "Vytvaram proces p1.\n");
    if((p1PID=fork())<0){
        dprintf(zadanieErrFD,"Chyba: Vytvorenie procesu p1 sa nepodarilo.\n");
        FatalError();
    }else if(p1PID == 0){
        dprintf(zadanieOutFD, "Vytvaranie procesu p1 sa podarilo.\n");
        if(execl("./proc_p1", "./proc_p1" ,pipeR1Write, (char *)NULL) < 0){
            dprintf(zadanieErrFD,"Chyba: Spustenie procesu p1 sa nepodarilo.\n");
            FatalError();
        }
    }
    dprintf(zadanieOutFD, "Pustenie procesu p1 sa podarilo.\n");
}


void procP2(char pipeR1Write[LENGHT]){
    dprintf(zadanieOutFD, "Vytvaram proces p2.\n");
    if((p2PID=fork())<0){
        dprintf(zadanieErrFD,"Chyba: Vytvorenie procesu p2 sa nepodarilo.\n");
        FatalError();
    }else if(p2PID == 0){
        dprintf(zadanieOutFD, "Vytvaranie procesu p2 sa podarilo.\n");
        if(execl("./proc_p2", "./proc_p2", pipeR1Write, (char *)NULL) < 0){
            dprintf(zadanieErrFD,"Chyba: Spustenie procesu p2 sa nepodarilo.\n");
            FatalError();
        }
    }
    dprintf(zadanieOutFD, "Pustenie procesu p2 sa podarilo.\n");
}


void procPr(char p1PID[LENGHT],char p2PID[LENGHT],char pipeR1Read[LENGHT],char pipeR2Write[LENGHT]){
    dprintf(zadanieOutFD, "Vytvaram proces pr.\n");
    if((prPID=fork())<0){
        dprintf(zadanieErrFD,"Chyba: Vytvorenie procesu pr sa nepodarilo.\n");
        FatalError();
    }else if(prPID == 0){
        dprintf(zadanieOutFD, "Vytvaranie procesu pr sa podarilo.\n");
        if(execl("./proc_pr", "./proc_pr", p1PID, p2PID, pipeR1Read, pipeR2Write, (char *)NULL) < 0){
            dprintf(zadanieErrFD,"Chyba: Spustenie procesu pr sa nepodarilo.\n");
            FatalError();
        }
    }
    dprintf(zadanieOutFD, "Pustenie procesu pr sa podarilo.\n");
}
void procT(char pipeR2Read[], char SM1[], char S1[]){
    dprintf(zadanieOutFD, "Vytvaram proces t.\n");
    if((tPID=fork())<0){
        dprintf(zadanieErrFD,"Chyba: Vytvorenie procesu t sa nepodarilo.\n");
        FatalError();
    }else if(tPID == 0){
        dprintf(zadanieOutFD, "Vytvaranie procesu t sa podarilo.\n");
        if(execl("./proc_t", "./proc_t", pipeR2Read, SM1, S1, (char *)NULL) < 0){
            dprintf(zadanieErrFD,"Chyba: Spustenie procesu t sa nepodarilo.\n");
            FatalError();
        }
    }
    dprintf(zadanieOutFD, "Pustenie procesu t sa podarilo.\n");
}

void procS(char SM1[], char S1[], char SM2[], char S2[]){
    dprintf(zadanieOutFD, "Vytvaram proces s.\n");
    if((sPID=fork())<0){
        dprintf(zadanieErrFD,"Chyba: Vytvorenie procesu s sa nepodarilo.\n");
        FatalError();
    }else if(sPID == 0){
        dprintf(zadanieOutFD, "Vytvaranie procesu s sa podarilo.\n");
        if(execl("./proc_s", "./proc_s", SM1, S1, SM2, S2, (char *)NULL)<0){
            dprintf(zadanieErrFD,"Chyba: Spustenie procesu s sa nepodarilo.\n");
            FatalError();
        }
    }
    dprintf(zadanieOutFD, "Pustenie procesu s sa podarilo.\n");
}

void procD(char shmSM2[], char S2[], char serverPort1[]){
    dprintf(zadanieOutFD, "Vytvaram proces d.\n");
    if((dPID=fork())<0){
        dprintf(zadanieErrFD,"Chyba: Vytvorenie procesu d sa nepodarilo.\n");
        FatalError();
    }else if(dPID == 0){
        dprintf(zadanieOutFD, "Vytvaranie procesu d sa podarilo.\n");
        if(execl("./proc_d", "./proc_d", shmSM2, S2, serverPort1, (char *)NULL) < 0){
            dprintf(zadanieErrFD,"Chyba: Spustenie procesu d sa nepodarilo.\n");
            FatalError();
        }
    }
    dprintf(zadanieOutFD, "Pustenie procesu d sa podarilo.\n");
}

void procServ1(char serverPort1[], char serverPort2[]){
    dprintf(zadanieOutFD, "Vytvaram proces serv1.\n");
    if((serv1PID=fork())<0){
        dprintf(zadanieErrFD,"Chyba: Vytvorenie procesu serv1 sa nepodarilo.\n");
        FatalError();
    }else if(serv1PID == 0){
        dprintf(zadanieOutFD, "Vytvaranie procesu serv1 sa podarilo.\n");
        if(execl("./proc_serv1", "./proc_serv1", serverPort1, serverPort2, (char *)NULL) < 0){
            dprintf(zadanieErrFD,"Chyba: Spustenie procesu serv1 sa nepodarilo.\n");
            FatalError();
        }
    }
    dprintf(zadanieOutFD, "Pustenie procesu serv1 sa podarilo.\n");
}

void procServ2(char serverPort2[]){
    dprintf(zadanieOutFD, "Vytvaram proces serv2.\n");
    if((serv2PID=fork())<0){
        dprintf(zadanieErrFD,"Chyba: Vytvorenie procesu serv2 sa nepodarilo.\n");
        FatalError();
    }else if(serv2PID == 0){
        dprintf(zadanieOutFD, "Vytvaranie procesu serv2 sa podarilo.\n");
        if(execl("./proc_serv2", "./proc_serv2", serverPort2, (char *)NULL) < 0){
            dprintf(zadanieErrFD,"Chyba: Spustenie procesu serv2 sa nepodarilo.\n");
            FatalError();
        }
    }
    dprintf(zadanieOutFD, "Pustenie procesu serv2 sa podarilo.\n");
}

void killAllProcesses(){
    kill(p1PID, SIGKILL);
    kill(p2PID, SIGKILL);
    kill(prPID, SIGKILL);
    kill(tPID, SIGKILL);
    kill(sPID, SIGKILL);
    kill(dPID, SIGKILL);
    kill(serv1PID, SIGKILL);
    kill(serv2PID, SIGKILL);
}