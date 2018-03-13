#ifndef _MYMALLOC_H
#define _MYMALLOC_H

	#include <stdio.h>
	#include <stdlib.h>

	#define malloc(x) mymalloc((x), __FILE__, __LINE__)
	#define free(x) myfree((x), __FILE__, __LINE__)

	void *mymalloc(int bsize, char* file, int line);
	void myfree(void* p, char* file, int line);

#endif

typedef struct header{
	signed short int size;
	short int free;
	struct header * next;
}memNode;

void FirstInitial();
void Merge(memNode* prev, memNode* ptr);

