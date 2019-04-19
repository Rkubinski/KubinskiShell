#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include "interpreter.h"
#include "pcb.h"
#include "IOSCHEDULER.h"


//PARTITION is private but global
struct PARTITION
	{
	int total_blocks;
	int block_size;
	};
struct PARTITION part;

//global private variable pointing to the current partition
FILE*currentPartition;
char partitionName[1000];
int currentPartitionLocation;	//this will index the blocks of the data partition
char DATA [1000];

struct fpToFAT 
	{
	int fp_index;
	int fat_index;
	} map[5];

struct FAT {
 char *filename;
 int file_length;
 int blockPtrs[10];
 int current_location;		//should be initialized to -1 when not used
} fat[20];




char * block_buffer;
FILE *fp[5];


void initIO()
	{
		createQueue();
		block_buffer = NULL;
		for (int i=0;i<5;i++)
			{
				fp[i]=NULL;
				map[i].fp_index=-1;
				map[i].fat_index=-1;

			}
		part.total_blocks=0;
		part.block_size=0;

		



	for (int i=0;i<20;i++)
			{	
				fat[i].filename=NULL;
				//fat[i].filename= (char*)calloc(100 + 1, sizeof(char));
				fat[i].file_length=0;
				for (int j=0;j<10;j++)
					{
						fat[i].blockPtrs[j]=-1;

					}
				fat[i].current_location=-1;
			}


	}


int findFatTop()
	{int idx=-1;
		for (int i=0 ; i<20;i++)
			{

			if (fat[i].file_length==0)
				{
					idx=i;
					break;}
			}
	
	return idx;
	}
void printMAP()
	{

		for (int i=0 ; i<5;i++)
			{
			printf("Map index: %d Fat index: %d fp index: %d\n",i,map[i].fat_index,map[i].fp_index);
				
			}
	}

int findMapTop()
	{int idx=-1;
		for (int i=0 ; i<5;i++)
			{
			if (map[i].fat_index==-1)
				{idx=i;
					break;}
			}
	return idx;
	}
int searchMap(int fatLocation)
	{int idx=-1;
		for (int i=0 ; i<5;i++)
			{
			if (map[i].fat_index==fatLocation)
				{idx=i;}
			}
	return idx;
	}

int partition(char *name,int totalblocks, int blocksize)
	{	

		DIR* dir = opendir("PARTITION");
		if (dir)
		{
    	/* Directory exists. */
    	closedir(dir);
		}
		else if (ENOENT == errno)
		{
    	/* Directory does not exist. */
			system("mkdir PARTITION");
		}
		
		char newfile[1000]="";
		char path[1000]="";

		strcat(newfile,"touch ");
		strcat(path,"PARTITION/"); 
		strcat(path,name);
		strcat(path,".txt");
		strcat(newfile,path);
		system(newfile);
		
		currentPartition = fopen(path,"w");
			if (currentPartition==NULL) return 0;

		fprintf(currentPartition, "%d\n", totalblocks);
		fprintf(currentPartition, "%d\n", blocksize);
		
		for (int i =0; i<20;i++)
			{

			fprintf(currentPartition,"%s ",fat[i].filename);	
			fprintf(currentPartition,"%d ",fat[i].file_length);	
			for (int j=0;j<10;j++)
				{

				fprintf(currentPartition,"%d ",fat[i].blockPtrs[j]);	
				}
			fprintf(currentPartition,"%d",fat[i].current_location);	
			}
	
		


		for (int i=0;i<blocksize*totalblocks;i++)
			{fprintf(currentPartition,"%c",'0');}
		
		
		fclose(currentPartition);
		return 1;
		
	}


int mount(char *name)
	{	char r[1000]="";
		strcat(r,"PARTITION/");
		strcat(r,name);
		strcat(r,".txt");
		currentPartition=fopen(r,"r+");
		if (currentPartition==NULL) return 0;
		strcpy(partitionName,name);
		int total_blocks=0;							
		int block_size=0;
		fscanf (currentPartition, "%d", &total_blocks);				//MAKE SURE TO STORE THESE INTO THE GLOBAL PARTITION
		part.total_blocks=total_blocks;
		fscanf (currentPartition, "%d", &block_size);
		part.block_size=block_size;
		for (int i =0; i<20;i++)
			{
			fscanf(currentPartition,"%s ",fat[i].filename);	
			fscanf(currentPartition,"%d ",&fat[i].file_length);	
			for (int j=0;j<10;j++)
				{
				fscanf(currentPartition,"%d ",&fat[i].blockPtrs[j]);	
				}
			fscanf(currentPartition,"%d",&fat[i].current_location);	
			}
		fseek(currentPartition,0,SEEK_SET);
		block_buffer=calloc(1,part.block_size);					// we malloc in mount because we dont always have the chance to in partition e.g: partition already exists
		printf("Partition %s mounted.\n",name);
		return 1;
	}
int findFpTop()
	{
	int idx=-1;
	for (int i=0;i<5;i++)
		{
			if (fp[i]==NULL)
				{idx=i;
				 break;}
		}
	return idx;
	}


int openfile(char*name)
	{
	int fatLocation=-1;
	for (int i=0;i<20;i++)
		{													//if the file is in FAT
			if (fat[i].file_length>0)
				{
			if (strcmp(fat[i].filename,name)==0)
				{
														//if file is in fat, then it is also in the map and the fp
				fatLocation=i;
				break;
				}
			}
		}
	if (fatLocation==-1)
		{		fatLocation=findFatTop();
				if (fatLocation==-1)
					{
					printf("No more space in FAT\n");
					return -1;
					}
				else {
					int map_index=findMapTop();	//this assumes that if theres an fp index, then theres a map index
					if (map_index== -1)					//FP is all filled up, no space in the map
						{
						printf("ERROR, no more spots in the fp[]\n");
						return -1;
						}

					else 
						{
						int fp_index=findFpTop();
						if (fp_index == -1)				//no spots in the fp 
							{printf("ERROR, no more spots in the fp[]\n");
							return -1;
							}
						//spot in fp and spot in map
						else
							{

							FILE *f = fopen(name,"w+");	//open file for reading OR writing, file is created if it doesnt exist
						
							
							fp[fp_index]=f;

							map[map_index].fp_index=fp_index;
							map[map_index].fat_index=fatLocation;
							fat[fatLocation].filename=(char *)calloc(strlen(name)+1,sizeof(char));	//allocating the space needed
							strcpy(fat[fatLocation].filename,name);
						}
					
					}
				
				
				
				}

				
							
		}
	


	return fatLocation;
	}

char *returnBlock() 
{
	return block_buffer;
}

void writePartition(char data[])
	{	
	fseek(currentPartition,0,SEEK_SET);		//position at beginning of file
	fprintf(currentPartition, "%d\n", part.total_blocks);
	fprintf(currentPartition, "%d\n", part.block_size);
		
	for (int i =0; i<20;i++)
		{
		fprintf(currentPartition,"%s ",fat[i].filename);	
		fprintf(currentPartition,"%d ",fat[i].file_length);	
		for (int j=0;j<10;j++)
			{
			fprintf(currentPartition,"%d ",fat[i].blockPtrs[j]);	
			}
		fprintf(currentPartition,"%d\n",fat[i].current_location);	
			//printf("Manual offset: %d\n",ftell(currentPartition));
		}

	fprintf(currentPartition, "\n");
	
	for (int i=0;i<strlen(data);i++)
		{
		fputc(data[i],currentPartition);
		}

	}

int readBlock(int file)
	{
	FILE *f = fp[map[searchMap(file)].fp_index];
	if (f==NULL)			//if the file is not open
		{
			f=fopen(fat[file].filename,"a+");
			fat[file].current_location=0;
		}
	char output[part.block_size+1];
	int location= fat[file].blockPtrs[fat[file].current_location];
	if (location == -1)	
		{
			location= currentPartitionLocation;
		}
	
	for (int i=0;i<part.block_size;i++)
		{

		output[i]=DATA[i+currentPartitionLocation];
		
		}
	fat[file].current_location++;
	writePartition(DATA);
	output[part.block_size]='\0';

	strcpy(block_buffer,output);
	printf("Read operation terminated\n");
	}






int writeBlock(int file,char *data)
	{
	
	printf("Writing block\n");
	int bs=part.block_size;
	char output [part.total_blocks*part.block_size];
	FILE *f = fp[map[searchMap(file)].fp_index];
	if (f==NULL)			//if the file is not open
		{
			f=fopen(fat[file].filename,"a+");
		}
    
    
   	 	
   	if (fat[file].current_location == -1)	//file never been opened before, we start from scratch
   		{	
   			currentPartitionLocation=0;	//we over write from the beginning
   			fat[file].current_location=0;
		}
	//if file was opened before, the current_location will be >-1 and the overall currentPartitionLocation will also be incremented

	if (strlen(data)+currentPartitionLocation>part.total_blocks*part.block_size)
   			{
   				printf("DATA too large, will not fit in the partition\n");
   			}
   	else 
   		{
   		if (currentPartitionLocation==0)
   			{
   			memset(output,0,part.total_blocks*part.block_size);

   			for (int i=0;i<(strlen(data));i++)			
				{output[i]=data[i];
				if (i%bs==0)		//we complete a block
					{
		 			//update the FAT fields 
		 			fat[file].blockPtrs[fat[file].current_location]=currentPartitionLocation;
		 			fat[file].file_length++;
		 			fat[file].current_location++;
		 			}
		 		currentPartitionLocation++;
				}
			
			for (int j=currentPartitionLocation;j<part.total_blocks*part.block_size-1;j++)
				{
				output[j]='0';
				}
			}
		else
			{

			
			
			fseek(f,0,SEEK_SET);
			
			int idx=0;
			char c=fgetc(f);
			while(!feof(f))
				{
				if (feof(f)) break;
				output[idx]=c;
				idx++;
				//printf("char fetched: %c\n",c);
				c=fgetc(f);
				
				}
			
			
			for (int i=0;i<(strlen(data));i++)
				{

				output[currentPartitionLocation]=data[i];
				if (currentPartitionLocation%bs==0)		//we complete a block
					{
		 			//update the FAT fields 
		 			fat[file].blockPtrs[fat[file].current_location]=currentPartitionLocation;
		 			fat[file].file_length++;
		 			fat[file].current_location++;
		 			}
		 		currentPartitionLocation++;
				}
			for (int j=currentPartitionLocation;j<part.total_blocks*part.block_size-1;j++)
				{
				output[j]='0';
				}
			}

		strcpy(DATA,output);
		}

	
	//writing into the actual file					
    fprintf(f,"%s",data);	//this is not touched after so any further writes will append to the actual file

    writePartition(output);	//we save everything except the data	

	
	
	printf("Finished write operation\n");

    


	}