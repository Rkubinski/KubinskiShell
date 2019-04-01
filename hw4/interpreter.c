#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "shell.h"
#include "shellmemory.h"
#include "kernel.h"
#include "memorymanager.h"



void help()
{printf("\nCOMMAND LIST\n-------------------\n");

{printf("help\n");
printf("quit\n");
printf("print VAR\n");
printf("set VAR VAL\n");
printf("run filename.txt\n");
printf("exec p1 p2 p3\n");
}
}
int quit() {return -1;}




void exec(char*command[], int args)
	{//checking for repeated args
		
		

		for (int i = 1; i<args;i++)
		{

		//check if input is valid
			FILE *f=fopen(command[i],"r");
			if (f==NULL)
				{printf("ERROR. %s not found.\n",command[i]);
			return;}
			else fclose(f);
		}
		
		int errcode;
		for (int i= 1; i<args;i++)
		{
		FILE *f = fopen(command[i],"r");
		errcode=launcher(f);

		if (errcode==0)
			{printf("ERROR in launcher\n");}
	
		

		}
		



		scheduler();
		
		
		
	}	






void runscript (char * script)
{FILE *f = fopen(script,"r");
if (f==NULL) 
	{printf("Invalid file\n");
return;
}	
char line[256];

while(fgets(line,sizeof(line),f))
{
	parseinput(line);
}


fclose(f);

}



int isCommand(char *command[], int args)
{
	
	
	if (strcmp(command[0],"help")==0 || strcmp(command[0],"quit")==0)
		{return 0;}
	else if(strcmp(command[0],"print")==0 || strcmp(command[0],"run")==0)
	{
		if (args==2)
			{return 0;}
		else 
			{printf("This command requires an input argument\n");
		return 2;}
	}
	else if(strcmp(command[0],"set")==0)
	{
		
		if (args==3)
			{return 0;}
		else 
			{printf("This command requires two input arguments\n");
		return 2;}
	}
	else if(strcmp(command[0],"exec")==0)
	{

		if(args<2 || args>4)
			{printf("exec command takes 1-3 arguments\n");
		return 2;}


		else{return 0;}
	}
	else return 1;
}


//here we assume that command[0] is the command
int interpreter(char * command[], int argcount)
{
	//printf("gang");
	int errCode=0;
	//if nothing is typed in, we can just renew the shell
	if (argcount==0)
		{return errCode;}
	
	

	//we check if the command is valid first
	
	int commCheck=isCommand(command, argcount);
	
	//we check if the command is valid or not
	if(commCheck==1)
		{printf("UNKNOWN COMMAND\n");
	return errCode;}
	else if (commCheck==2)
		{return errCode;}




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

	if(strcmp("exec",command[0])==0)
	{
		exec(command,argcount);

	}

	//we reset the array after we are done interpreting.
	for (int i=0;i<argcount;i++)
	{

		command[i]=NULL;}
		
		return errCode;}




