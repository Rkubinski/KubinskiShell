struct CPU 
	{
		FILE*IP;
		char IR[1000];
		int quanta;


	};
extern struct CPU c;

int isExecuting();

int run(int quanta);