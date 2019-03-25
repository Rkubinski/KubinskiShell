	#include "shell.h"
	#include "ram.h"
	#include "pcb.h"
	#include "cpu.h"
	#include <stdlib.h>
	#include <stdio.h>
	#include "memorymanager.h"
	#include <string.h>
	#include <unistd.h>

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
	int numberNodes()
	{int c=1;
		if (tail==NULL && head==NULL)
			{c=0;
				return c;}
		PCB *tmp=tail;
		
		while (tmp->next!=NULL)
		{
			tmp=tmp->next;
			c++;

		}

		return c;
	}

void deleteFile(FILE *p)
	{

	char path[1024];
    char result[1024];
    char finalpath[100];
    int index=0;

	int fd=fileno(p);
    sprintf(path, "/proc/self/fd/%d", fd);
    memset(result, 0, sizeof(result));
    readlink(path, result, sizeof(result)-1);


    for (int i=0;i<1024 &&result[i]!='\0';i++)
    	{

		if (result[i]=='/')
			{index=i;}

		}

	for (int i=0;i<1024 &&result[i]!='\0';i++)

		{
			finalpath[i]=result[i+index+1];

		}
	
	char command[1000];
	char newfile[1000];


	strcat(command,"rm ");
	strcat(command,"./BackingStore/");
	strcat(command,finalpath);
	
	system(command);
	memset(command,0,1000);
	}

void terminate()
	{	
		


		PCB *tmp=head;
		c.IP=NULL;
	
		deleteFile(tmp->PC);



		if (numberNodes()==1)
		{

					removeFILEPT(head->PC); //removing FILE * (s) from RAM

					fclose(head->PC);	//closing the file

					
					
					tail =NULL;
					head=NULL;
					free(tmp);
					tmp=NULL;

				}
				
				

				else{
					removeFILEPT(head->PC); //removing FILE * from RAM
					fclose(head->PC);	//closing the file
					PCB* beforeFirst=findBeforeFirst();	
					head=beforeFirst;
					head->next=NULL;
					free(tmp);	//freeing the malloc
					tmp=NULL;
				}	
	
	}

			void updateVictimTable(PCB*p,int frame)
			{
				PCB *tmp=tail;
				while (tmp->next !=NULL)
				{
				if (tmp!=p)		//if it is not the same as the PCB that is stealing the victim's spot
				{
					for (int i=0;i<10;i++)		//then we loop thru pagetable, checking which page has the frame we are victimizing
					{	//printf("Victim table index: %d, value: %d\n",i,tmp->pageTable[i]);
						if (tmp->pageTable[i]==frame)
							{	
								//printf("Victim Page Table Before Update\n");
								//for (int i=0;i<10;i++)
								//	{
								//		printf("Index: %d | Frame: %d\n",i,tmp->pageTable[i]);

								//	}
								tmp->pageTable[i]=-1;		//we mark that pageTable index as -1 since it no longer has a location in RAM
								//printf("Victim Page Table After Update\n");
								//for (int i=0;i<10;i++)
								//	{
								//		printf("Index: %d | Frame: %d\n",i,tmp->pageTable[i]);

								//	}
								return;
							}			
						}
					}
					tmp=tmp->next;

				}


			}

void handlePageFault()
	{	//printf("Handling page fault for file %p\n",head->PC);
		int frame=findFrame(head->PC);	
		int vicFrame=findVictim(head);			//we find a frame that doesnt belong to our process	
		//printf("Victim after call: %d\n",vicFrame);
		findPage(head->PC_page,head->PC); 		//we simulate retrieving the page
		updateFrame(frame, vicFrame, head->PC);	//these are the decision makers, they decide whether we victimize page or not
		updatePageTable(head,head->PC_page,frame,vicFrame);
		fseek(head->PC, 0, SEEK_SET); //we bring back the file pointer

	}



void scheduler()
	{

	int Flag;
	while(hasProcesses())	//checks if there are any PCBs in the queue.
	{

		if(!isExecuting())	//checks if CPU is executing or not
		{
			
			
			int frame=head->pageTable[head->PC_page];
			//printf("Looking at frame; %d for file: %p\n",head->pageTable[head->PC_page],head->PC);
			if (frame == -1)

			{
				handlePageFault();			//we handle page faults at beginning because 
				frame=head->pageTable[head->PC_page];
			}

			FILE *tmp=ram[frame]; 	
			
			c.IP=findPage(head->PC_page,tmp);	//copy the pointer incremented to the right location to the CPU IP pointer
			
			c.offset=head->PC_offset;			//copy the offset from the PCB
			Flag=run(c.quanta);					//runs for 2 instructions
			fseek(head->PC, 0, SEEK_SET); 		// we bring back the file pointer
			//if not at end of file 

			if (Flag==0)
				{	head->PC_offset=c.offset;
					//printf("updated offset after run: %d\n",head->PC_offset);
					
					if (head->PC_offset==4)									//if the offset is 4 then we need a task switch
						{

						if (head->PC_page+1>=head->pages_max) 				//if we have no more pages, we terminate
							{	
							//printf("Max pages: %d, PC_page: %d. Time to terminate!\n",head->pages_max,head->PC_page+1);
							//printf("terminating\n");
							terminate();
							

							}	
						else 
							{head->PC_page++;								//otherwise we start to use the next page
							head->PC_offset=0;								//we reset the offset
							int frame=head->pageTable[head->PC_page];
							if (frame>=0)			//we check if next page has a frame, if it is -1, it doesnt have a frame in RAM
								{
								
								//printf("Page: %d exists in ram at position: %d\n",head->PC_page,frame);
								head->PC=ram[frame];						//we simulate a copy of the page from the ram at the frame to the PC
								}										//ram[frame] should fseek-ed to the beginning so no need to reset
							else

								{	

									handlePageFault();
								}
					
							}		
						}

					//we reorder the nodes of PCB
				//printf("has processes: %d, number of nodes: %d\n",hasProcesses(),numberNodes());
				if (numberNodes()>1)
					{	

					PCB* beforeFirst=findBeforeFirst();
					PCB *tmp=head;
					head=beforeFirst;
					tmp->next=tail;
					tail=tmp;
					head->next=NULL;
					}
				
				//printf("test\n");
				}																//if we have 3 pages originally, only first 2 will be in RAM
			else 
				{

				terminate();}				//if our Flag indicates eof, we terminate
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