#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreter.h"
#include <unistd.h>


int parseinput(char comm[])
	{
	
	char tmp[200];
	char *words[100];
	int i,j,w=0;
	comm[strlen(comm) - 1] = '\0';	//replaces the \n at the end with a /0
	
	
	//skip the first bit of white spaces
	for(i=0; comm[i]==' ' && i<1000;i++);

	//keep going to the end of the command
	while(comm[i] != '\0' && i<1000)
		{
		//get the next word and store it in tmp
		for (j=0;comm[i] != '\0' && comm[i]!=' ' && i <1000; i++, j++)
		tmp[j]=comm[i];
		
		//end the string with \0
		tmp[j]= '\0';
		//convert the chars into a string and store it in the words array
		words[w] = strdup(tmp);
		
		w++;i++;}
	
	
	
	return interpreter(words,w);
	}

//make sure to account for carriage return
void shell()
	{
	char command[1000];
	int errorcode;

	printf("Welcome to the Kubinski shell\n");
	printf("Shell version 2.0 Updated February 2019\n");


	while(1){

		if (!isatty(0) && strcmp(command,"")==0)
			{exit(99);}
		printf("$");
		


		fgets(command,1000,stdin);
		
		errorcode=parseinput(command);

		if (errorcode==-1)
			{
			printf("bye!\n");			
			exit(99);}
	
		}
		

	}



