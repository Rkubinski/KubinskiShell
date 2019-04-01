#include <stdio.h>
#include <stdlib.h>


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
		int pages_max=0;
		
		
		return p;
	}

void initStruct(struct pcb * p)
	{

			p->PC_page=0;
			p->PC_offset=0;
			p->pages_max=0;
			for (int i=0;i<10;i++)
			{

				p->pageTable[i]=(-1);			//this marks uninitialized values in the PCB

			}


	}

	

	
