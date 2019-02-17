#include <stdio.h>
#include <stdlib.h>


struct pcb
	{FILE*PC;
	struct pcb *next;};
	
struct pcb * makePCB(FILE *f)
	{
		struct pcb *p = malloc(sizeof(struct pcb));
		p->PC= f;
		p->next=NULL;

		return p;
	}

	

	
