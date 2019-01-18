#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "shell.h"
#include "shellmemory.h"




void help()
	{printf("COMMAND LIST\n-------------------\n");
	
	{printf("help\n");
	 printf("quit\n");
	 printf("print VAR\n");
	 printf("set VAR VAL\n");
	 printf("run filename.txt\n");
	}
	}
int quit() {return -1;}

void runscript (char * script)
	{FILE *f = fopen(script,"r");
	if (f==NULL) 
		{printf("Invalid file\n");
		return;
		}	
	char line[256];

	while(fgets(line,sizeof(line),f))
		{parseinput(line);}
	
	fclose(f);
	
	}
bool isCommand(char *command[], int args)
	{
			
	if (strcmp(command[0],"help")==0 || strcmp(command[0],"quit")==0)
		{return true;}
	else if(strcmp(command[0],"print")==0 || strcmp(command[0],"run")==0)
		{
		if (args>1)
			{return true;}
		}
	else if(strcmp(command[0],"set")==0)
		{
		
		if (args>2)
			{return true;}
		}
	}


//here we assume that command[0] is the command
int interpreter(char * command[], int argcount)
	{
	int errCode=0;

	
	

	//we check if the command is valid first
	
	
	
	if(!isCommand(command, argcount))
		{printf("UNKNOWN COMMAND\n");
		return errCode;}


	if (strcmp("help",command[0])==0)
		{
		help();
		}	

	//exit if command is quit
	if (strcmp("quit", command[0])==0) 
		{errCode=quit();}
	
	if (strcmp("set", command[0])==0) 
		{
		insert(command[1],command[2]);
		}
	if (strcmp("print", command[0])==0) 
		{
		
		print(command[1]);
		}
	if(strcmp("run",command[0])==0)
		{
		
		runscript(command[1]);
				

		}
	//we reset the array after we are done interpreting.
	for (int i=0;i<argcount;i++)
		{command[i]=NULL;}
	
	return errCode;}



