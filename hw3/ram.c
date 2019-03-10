#include <stdio.h>

FILE *ram[10];

int findTop()
	{int top=0;
		for (int i=0;i<10;i++)
			{	
			if (ram[i]==NULL) top=i; break; 
			}
	return top;
	}


void addToRAM(FILE *f)
	{
	int top=findTop();
	ram[top]=f;

	}

void removeFILEPT(FILE*f)
	{
	for (int i=0; i<10;i++)
		{
		if (ram[i]==f)
			{ram[i]=NULL;}

		}

	}