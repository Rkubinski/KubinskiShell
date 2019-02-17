#include <stdio.h>
#include "cpu.h"
#include <string.h>
#include "shell.h"
#include <stdlib.h>



struct CPU c={.quanta=2};

int isExecuting()
	{

		if (c.quanta<2 ||c.IP ==NULL)	//if quanta is decremented to 0, then we have finished with some process.
										//second case is if IP hasnt been initialized yet 
			{c.quanta=2;		//reset the quanta
				return 0;}
		else return 1;

	}

int run (int quanta)
	{char command[1000];
		int flag=0;
		for (int i=0;i<c.quanta;c.quanta--)
			{
			
			if(fgets(command, sizeof(command), c.IP)!=NULL)
				{	
					parseinput(command);
					memset(command, 0, 255);
				
				}
			
			
			else {
				
				flag=1;
			
			}
			//otherwise terminate the process/ PCB (add this later)
			//if file isnt terminated the update PCB with value of IP 
			
			}
		
		return flag;



	}





