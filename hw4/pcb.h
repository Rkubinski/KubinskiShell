#include <stdio.h>


typedef struct pcb
	{FILE*PC;
	struct pcb *next;
	int PC_page,PC_offset,pages_max;
	int pageTable[10];
	} PCB;

PCB* makePCB(FILE *f);

void initStruct(PCB* p);