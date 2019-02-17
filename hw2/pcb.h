#include <stdio.h>

typedef struct pcb
	{FILE*PC;
	struct pcb *next;} PCB;

PCB* makePCB(FILE *f);