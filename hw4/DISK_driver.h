#include <stdio.h>
int partition(char *name,int blocksize, int totalblocks);
void initIO();
int readBlock(int file);
int openfile(char*name);
int writeBlock(int file,char *data);
int mount(char *name);
char *returnBlock();