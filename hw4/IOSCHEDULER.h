#include <stdio.h>


char *IOscheduler(char *data, PCB *ptr, int cmd);
void mountPartition(char* command[]);
void updateActiveFile(char *name);
void enqueue(PCB *p, char *d, int c);
void createQueue();
void dequeue();
