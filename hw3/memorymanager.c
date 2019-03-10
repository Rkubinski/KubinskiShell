#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "kernel.h"
#include <unistd.h>
#include <time.h>
#include "ram.h"


int launcher(FILE *p, char * filename)

{
//we build the command that we pass to system
	char command[1000];
	char *newfile="BackingStore/";
	strcat(newfile,filename);



	strcat(command,"cp ");
	strcat(command,filename);
	strcat(command,newfile);
	system(command);

	fclose(p);


	FILE *f=fopen(newfile,"r");


}

int countTotalPages(FILE *f)
	{ int counter=0;

	  char line[1000];
		while(fgets(line,sizeof(line),f))
		{
  		counter++;
		}

	counter=ceil(counter/4);
	return counter;

	}

FILE *findPage(int pageNumber, FILE*f)
	{
		char *line;
		FILE *fp2 = fdopen (dup (fileno (f)), "r");
		for (int i=0;i<4*pageNumber;i++)
			{fgets(line,1000,fp2);}

	return fp2;


	}

int findFrame(FILE*page)
	{
	int idx=-1;
		for(int i=0;i<10;i++)
			{
				if (ram[i]!=NULL)
					{	
						idx=i;
						
					}

			}

	return idx;

	}

int findVictim(PCB *p)
	{
	int n;
	srand(time(NULL));
	n = rand() % 10;
 	//remember to check if in PCB


	return n;
	}

int updateFrame(int frameNumber, int victimFrame, FILE *page)
	{


	}

int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame)
	{


	}