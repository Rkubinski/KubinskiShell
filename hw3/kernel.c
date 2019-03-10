#include "shell.h"
#include "ram.h"
#include "pcb.h"
#include "cpu.h"
#include <stdlib.h>
#include <stdio.h>
#include "memorymanager.h"

PCB *head,*tail;
FILE *ram[10];

void clearRAM()
	{
		for (int i =0;i<10;i++)
			{
				ram[i]=NULL;
			}

	}
void prepBackStore()
	{	
		system("rm -r BackingStore");
		system("mkdir BackingStore");
	}

void boot()
	{
		clearRAM();
		prepBackStore();


	}

void addToReady(PCB* p)
	{
	if (head==NULL && tail==NULL)
		{head=p;
			tail=p;}
	else
		{
		p->next=tail;
		tail=p;

		}

	}

void myinit(FILE *f)
	{
	addToRAM(f);
	FILE* fp=f;				//the goal was to have a different pointer in RAM and a different pointer in the PCB
	PCB * p=makePCB(fp);
	addToReady(p);
	}
int hasProcesses()
	{
		if (head !=NULL && tail!=NULL) return 1;
		else return 0;

	}
PCB* findBeforeFirst()
	{
		PCB *tmp=tail;
		while (tmp->next !=head)
			{
				tmp=tmp->next;

			}
		return tmp;

	}

void updateVictimTable(PCB*p,int frame)
	{
		PCB *tmp=tail;
		while (tmp->next !=NULL)
			{
				if (tmp!=p)		//if it is not the same as the PCB that is stealing the victim's spot
					{
					for (int i=0;i<10;i++)		//then we loop thru pagetable, checking which page has the frame we are victimizing
						{
						if (tmp->pageTable[i]==frame)
							{
								tmp->pageTable[i]=-1;		//not sure if making NULL is the right approach
								break;
							}			
						}
					}
			tmp=tmp->next;

			}
	

	}

int numberNodes()
	{int c=1;

		PCB *tmp=tail;
		while (tmp->next!=NULL)
			{
				tmp=tmp->next;
				c++;

			}

		return c;
	}


void scheduler()
	{
	int eofFlag;
	while(hasProcesses())	//checks if there are any PCBs in the queue.
		{

		if(!isExecuting())	//checks if CPU is executing or not
			{

			c.IP=head->PC;	//copy PCB PC pointer to CPU IP pointer
			eofFlag=run(c.quanta);	//runs for 2 instructions
			
			//if not at end of file, we place PCB back into queue
			if (eofFlag==0)
				{
				if (numberNodes()==1)
				{head->PC=c.IP;

				}

				else{ 
				head->PC=c.IP;	//update PCB pointer with IP value

				PCB* beforeFirst=findBeforeFirst();
				
				PCB *tmp=head;
				head=beforeFirst;
				
				tmp->next=tail;
				tail=tmp;
				head->next=NULL;
				}
			}
			else
				{
				

				PCB *tmp=head;
				c.IP=NULL;
				if (numberNodes()==1)
					{
				
					removeFILEPT(head->PC); //removing FILE * from RAM
				
					fclose(head->PC);	//closing the file
				
					
					
					tail =NULL;
					head=NULL;
					free(tmp);
					}
				
				
			
				else{
					removeFILEPT(head->PC); //removing FILE * from RAM
					fclose(head->PC);	//closing the file
					PCB* beforeFirst=findBeforeFirst();	
					head=beforeFirst;
					head->next=NULL;
					free(tmp);	//freeing the malloc
				}	
			

				
			
				



				}
				 
			


			}
				

		}

	
		
	}

	




void main()
{

boot();
printf("Kernel 1.0 loaded!\n");

shell();
//instantiate all data structures
}