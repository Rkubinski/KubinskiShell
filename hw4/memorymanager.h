#include <stdio.h>


int filecounter;

int launcher(FILE *p);

int countTotalPages(FILE *f);

FILE *findPage(int pageNumber, FILE*f);

int updateFrame(int frameNumber, int victimFrame, FILE *page);

int findFrame(FILE*page);

int findVictim(PCB *p);

int updatePageTable(PCB *p, int pageNumber, int frameNumber, int victimFrame);