#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>





char* map[1000][2];
int topofMap; 


int insert (char *k, char *v)
	{
	bool exists=false;
	//check if it exists first
	
	for (int i=0; i<topofMap;i++)
		{
		if (strcmp(map[i][0], k)==0)
			{map[i][1]=v;
			exists=true;
			break;}
		}
	
	if (exists==false)
		{
		map[topofMap][0]= k;
		map[topofMap][1]=v;
		topofMap++;
			
		}
	return 0;
	}

int print(char *k)
	{
	
	bool exists=false;
	for (int i=0; i<topofMap;i++)
		{
		
		if (strcmp(map[i][0], k)==0)
			{printf("%s\n",map[i][1]);
			exists=true;
			break;}
			}
	if(exists==false)
		{printf("Variable does not exist.\n");}
		
		
	return 0;
	}


