#include <stdlib.h>
#include <stdio.h>
#include "mymalloc.h"

static char myblock[5000];
memNode *head = (memNode*)myblock;

void FirstInitial(){
	head->size = 5000 - sizeof(memNode);
	head->next = NULL;
	head->free = 1;
}

void *mymalloc(int bsize, char *file, int line){
	
	if (bsize == 0 || bsize > 4984){
		fprintf(stderr, "Cannot malloc such int of (%d) bytes in FILE: (%s) on LINE: (%d)\n", bsize, file, line);
		return 0;
	}

	memNode *ptr = head;
	int leftSpace;

	if (!(head->size)){
		FirstInitial();
	}
	
	while (ptr->next != NULL){
		if (ptr->free == 1){
			if ((ptr->size) > (bsize + sizeof(memNode))){
				ptr->free = 0;
				leftSpace = (ptr->size) - (bsize + sizeof(memNode));
				memNode *new = (memNode*)((char*)(ptr) + sizeof(memNode) + bsize);
				new->size = leftSpace;
				new->free = 1;
				ptr->size = bsize;
				new->next = ptr->next;
				ptr->next = new;
				return (void*)ptr;			
			}
			else if((ptr->size) >= bsize){
				ptr->free = 0;
				return (void*)ptr;
			}
		}
		ptr = ptr->next;
	}
	if(ptr->free == 1){
	  if ((ptr->size) > (bsize + sizeof(memNode))){
		  ptr->free = 0;
		  leftSpace = (ptr->size) - (bsize + sizeof(memNode));
		  memNode *new = (memNode*)((char*)(ptr) + sizeof(memNode) + bsize);
		  new->size = leftSpace;
		  new->free = 1;
		  ptr->size = bsize;
		  new->next = ptr->next;
		  ptr->next = new;
		  return (void*)ptr;			
	  }
	  else if((ptr->size) >= bsize){
		  ptr->free = 0;
		  return (void*)ptr;
	  }
	}
	
	fprintf(stderr, "Memory depleted.  No such space for (%d) bytes in FILE: (%s) on LINE: (%d)\n", bsize, file, line);
	return 0;
}

void Merge(memNode *ptr, memNode *prev){
	if (prev != NULL && ptr->next != NULL){
		if ((prev->free) == 1 && (ptr->next->free) == 1){
			prev->size = (prev->size) + (ptr->size) + (ptr->next->size) + (2 * sizeof(memNode));
			prev->next = ptr->next->next;
		}
		else if ((prev->free) == 0 && (ptr->next->free) == 1){
			ptr->size = (ptr->size) + (ptr->next->size) + sizeof(memNode);
			ptr->next = ptr->next->next;
		}
		else if ((prev->free) == 1 && (ptr->next->free) == 0){
			prev->size = (prev->size) + (ptr->size) + sizeof(memNode);
			prev->next = ptr->next;
		}
	}
	else if (prev == NULL && ptr->next != NULL){
		if (ptr->next->free == 1){
			ptr->size = (ptr->size) + (ptr->next->size) + sizeof(memNode);
			ptr->next = ptr->next->next;
		}
	}
	else if (prev != NULL && ptr->next == NULL){
		if (prev->free == 1){
			prev->size = (prev->size) + (ptr->size) + sizeof(memNode);
			prev->next = ptr->next;
		}
	}	
}
		


void myfree(void *search, char *file, int line){
	if (search == NULL){
		fprintf(stderr, "Cannot free NULL pointer in FILE: (%s) on LINE: (%d)\n", file, line);
		return;
	}

	memNode *ptr = head;
	memNode *prev = NULL;
	
	while(ptr != NULL){
		if((void*)ptr == search){
			break;
		}
		prev = ptr;
		ptr = ptr->next;
		if(ptr == NULL){
			fprintf(stderr, "Pointer not found in FILE: (%s) on LINE: (%d)\n", file, line);
			return;
		}
	}
	
	if(ptr->free == 0){
		ptr->free = 1;
		Merge(ptr, prev);
		return;
	}
	else {
		if(ptr->free == 1){
			fprintf(stderr, "Pointer has been already freed in FILE: (%s) on LINE: (%d)\n", file, line);
			return;
		}
		else{
			fprintf(stderr, "Pointer not found in FILE: (%s) on LINE: (%d)\n", file, line);
			return;
		}
	}
}
