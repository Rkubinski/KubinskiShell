#include <stdio.h>
#include "DISK_driver.h"
#include "pcb.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>

char activeFile[1000];
struct Queue* queue;

struct queueElement
	{
		PCB *ptr;
		char *data;
		int cmd;

	};
struct Queue 
{ 
    int front, rear, size; 
  
    struct queueElement* array; 
}; 

void createQueue() 
{ 
    queue = (struct Queue*) malloc(sizeof(struct Queue)); 
    queue->front = queue->size = 0;  
    queue->rear = 10 - 1;  // This is important, see the enqueue 
    queue->array = malloc(10*(sizeof(struct queueElement)));
    
} 

int isFull(struct Queue* queue) 
{  return (queue->size == 10);  }
int isEmpty(struct Queue* queue) 
{  return (queue->size == 0); } 

void enqueue(PCB*p, char*d, int c) 
{ 
	struct queueElement e;
	e.ptr=p;
	e.data=d;
	e.cmd=c;
    if (isFull(queue)) 
        return; 
    queue->rear = (queue->rear + 1)%10; 
    
    queue->array[queue->rear]=e;
    queue->size = queue->size + 1; 
    
} 

void dequeue() 
{ 
    if (isEmpty(queue)) 
        return INT_MIN; 
  
    queue->front = (queue->front + 1)%10; 
    queue->size = queue->size - 1; 
   
}


void updateActiveFile(char *name)
	{
		strcpy(activeFile,name);
	}

char *IOscheduler(char *data, PCB *ptr, int cmd)
	{
		char * ret;
		char *a=activeFile;
		//printf("activefile from IOscheduler prior to openfile(): %s\n",a);

		int locus=openfile(a);	
		//printf("activefile from IOscheduler post openfile(): %s\n",a);
		if (locus == -1 )
			{printf("ERROR. Couldnt find a FAT allocation or FP.");
			return "";}
		else{
			if (cmd == 1)	//write
			{


			
			writeBlock(locus,data);			//this updates the FAT
			
			}
			else 		//read
			{readBlock(locus);


			 ret= returnBlock();
			 //printf("return in IOscheduler : %s\n",ret);
			 return ret;

			}
		}
		
	
	}

void mountPartition(char* command[])
	{		
		
		char filename[1000];
		memset(filename,0,1000);
		strcat(filename,"PARTITION/");
		strcat(filename,command[1]);
		strcat(filename,".txt");
		

		FILE*f =fopen(filename,"r");
		if (f!=NULL)		//if the file already exists
			{	
				fclose(f);

				mount(command[1]);
				
				}

		else
			{	
				

				partition(command[1],atoi(command[2]),atoi(command[3])); //we pass the name and the total_blocks along with block_size
				mount(command[1]);

				}										 //we mount this partition
		
	}