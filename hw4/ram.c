#include <stdio.h>
#include "kernel.h"







void removeFILEPT(FILE*f)
	{
	for (int i=0; i<10;i++)
		{
		if (ram[i]==f)
			{ram[i]=NULL;}

		}

	}