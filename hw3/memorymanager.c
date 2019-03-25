#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "kernel.h"
#include <unistd.h>
#include <time.h>
#include "ram.h"




int countTotalPages(FILE *f)
{ int counter=0;

	char line[1000];
	while(fgets(line,1000,f)!=NULL)
	{
		counter++;
	}
	
	fseek(f, 0, SEEK_SET);
	
	 counter = counter/4 + (counter % 4 != 0);
	
	

	return counter;

}


FILE *findPage(int pageNumber, FILE*f)
{
	char line[1000];
	
	for (int i=0;i<pageNumber*4; i++)
		{
		fgets(line,1000,f);
		}

	
return f;

}
int findFrame(FILE*page)
{
	int idx=-1;
	for(int i=0;i<10;i++)
	{
		if (ram[i]==NULL)
		{	
			idx=i;
			
			
			break;

		}

	}

	return idx;

}
int inPageTable(PCB *p,int n)
{int idx=-1;
	for (int i=0;i<10;i++)
	{
		if (p->pageTable[i]==n)
		{idx=i;}

	}
	return idx;		
}
int generateRandom()
{
	int n;
	//srand(time(NULL));
	n = rand() % 10;
	
	return n;
}

int findVictim(PCB *p)
{
	int n = generateRandom();
	
 	//while the pageTable has a frame at index n, we keep generating a new number
	while (inPageTable(p,n)>=0)
	{
		n=(n+1)%10;

	}
	
	return n;
}

int updateFrame(int frameNumber, int victimFrame, FILE *page)
{
	//printf("Ram before update: \n");
	//for (int i = 0 ; i<10;i++)
	//	{printf("Ram index: %d, value: %p\n",i,ram[i]);}

	if (frameNumber==-1)
		{
		
		//printf("Victimization. Storing page from %p in frame: %d\n",page,victimFrame);
		ram[victimFrame]=page;}
	else 
		{ram[frameNumber]=page;
			//printf("Empty spot found. Storing page from %p at index: %d\n",page,frameNumber);
		}

	//printf("Ram after update: \n");
	//for (int i = 0 ; i<10;i++)
	//	{printf("Ram index: %d, value: %p\n",i,ram[i]);}



}






int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame)
{/*
	printf("Page Table Before Update\n");
								for (int i=0;i<10;i++)
									{
										printf("Index: %d | Frame: %d\n",i,p->pageTable[i]);

									}*/
	if (frameNumber==-1)
		{p->pageTable[pageNumber]=victimFrame;

			updateVictimTable(p, victimFrame);
			p->PC=ram[victimFrame];			//we simulate the updating of the PC
		}
		else 
		{
			p->pageTable[pageNumber]=frameNumber;
			p->PC=ram[frameNumber];
		}
/*	printf("Page Table After Update\n");
		for (int i=0;i<10;i++)
									{
										printf("Index: %d | Frame: %d\n",i,p->pageTable[i]);

									}*/


	}
char * extractName(FILE*p)
	{

	char path[1024];
    char result[1024];
    char *finalpath;
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
	//finalpath[i+1]='\0';
	return finalpath;
	}

int launcher(FILE *p)

{
	

    
  	PCB * pp=makePCB(p);
	initStruct(pp);			//we make all unused indexes == -1 to mark that they are not used. This is better than using 0
	addToReady(pp);

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
	
	
	

	
	strcat(newfile,"BackingStore/");
	strcat(newfile,finalpath);
	strcat(command,"cp ");
	strcat(command,finalpath);
	strcat(command," ");
	strcat(command,newfile);

	system(command);			//WE COPY THE FILE INTO THE BACKING STORE 

	fclose(p);					//we close the old file and open the new one


	FILE *f=fopen(newfile,"r");



	int pages=countTotalPages(f);
	//printf("Number of pages: %d\n",pages);
	//printf("New file path: %s\n",newfile);

	char line[1000];
	int frame;
	int vic;



	//split the file into pages
	for (int i=0;i<2&&i<pages;i++)		//either load two pages or one page, depending which is smaller
										//if pages==1 then we only load one page
	{	
		findPage(i,f);					//this gets the page from the file

		frame=findFrame(f);					//this stores the page in RAM
		vic=findVictim(pp);
		updateFrame(frame,vic,f);
		updatePageTable(pp,i,frame,vic);

		pp->pageTable[i]=frame;

		fseek(f, 0, SEEK_SET); 			// instead of duplicating the file pointer we just bring it back to the start after every search
	}
	pp->pages_max=pages;
	memset(command,0,1000);
	memset(newfile,0,1000);
	//printf("Count of pages: %d\n",pp->pages_max);
	

}




