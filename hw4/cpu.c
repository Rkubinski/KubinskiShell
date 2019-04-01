#include <stdio.h>
#include "cpu.h"
#include <string.h>
#include "shell.h"
#include <stdlib.h>



struct CPU c={.quanta=2,
			 .offset=0};

int isExecuting()
	{

		if (c.quanta<2 ||c.IP ==NULL)	//if quanta is decremented to 0, then we have finished with some process.
										//second case is if IP hasnt been initialized yet 
			{c.quanta=2;		//reset the quanta
				return 0;}
		else return 1;

	}

int run (int quanta)
	{	char command[1000];
		int flag=0;

		//we pre-increment the file pointer every time 
		for (int i=0;i<c.offset;i++)
			{
				fgets(command,sizeof(command),c.IP);
				memset(command, 0, 255);
			}


		for (int i=0;i<c.quanta;c.quanta--)
			{
			
			if (c.offset==4)		//after the break,we need to save the offset into the PCB and change the cpu offset to 0. 
									//we return 0 immediately to indicate that we need a task switch
									//we will then check to see what the offset is 
				{

				
				return flag;}

			if(fgets(command, sizeof(command), c.IP)!=NULL &&strcmp(command,"")!=10)		//we check if strcmp is 10 because that means its only a new line
				{	
					
					parseinput(command);
					
					memset(command, 0, 255);
					c.offset++;
				}
			
			
			else {
				
				flag=1;
				return flag;
			}
			//otherwise terminate the process/ PCB (add this later)
			//if file isnt terminated the update PCB with value of IP 
			
			}
		
		return flag;



	}





