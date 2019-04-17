#include <stdio.h>
#include "pcb.h"	
extern FILE *ram[10];
void myinit(FILE*f);
void addToReady(PCB*p);
void scheduler();
void kernelTakeOver(char *command[], int size);
void updateVictimTable(PCB *p,int frame);
void fileIO(char* command[], int size);

