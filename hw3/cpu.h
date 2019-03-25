struct CPU 
	{
		FILE*IP;
		char IR[1000];
		int quanta;
		int offset;

	};
extern struct CPU c;

int isExecuting();

int run(int quanta);