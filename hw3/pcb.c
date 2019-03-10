#include <stdio.h>
#include <stdlib.h>
#include "memorymanager.h"


struct pcb
	{FILE*PC;
	struct pcb *next;
	int PC_page,PC_offset,pages_max;
	int pageTable[10];
	};
	
struct pcb * makePCB(FILE *f)
	{
		struct pcb *p = malloc(sizeof(struct pcb));
		p->PC= f;
		p->next=NULL;
		int PC_page=0;
		int PC_offset=0;
		int pages_max=countTotalPages(f);
		return p;
	}

	

	
