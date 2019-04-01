#include <stdio.h>
#include "pcb.h"	
extern FILE *ram[10];
void myinit(FILE*f);
void addToReady(PCB*p);
void scheduler();
void updateVictimTable(PCB *p,int frame);