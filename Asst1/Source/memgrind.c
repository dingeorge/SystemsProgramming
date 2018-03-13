#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <time.h>
#include "mymalloc.h"

void testA(){//malloc 1 byte and immediately free it (150 times)
	char *ptr = NULL; //malloc pointer
	int mallocA = 0; // total times malloc will be called
	int freeA = 0; // total times free will be called
	
	struct timeval start; // timestamp structure
	struct timeval end;

	unsigned long average = 0; // average time for 100 iterations
	
	int i = 0, j = 0;

	for(i = 0; i < 100; i++){
		gettimeofday(&start, 0);
		for(j = 0; j < 150; j++){
			ptr = malloc(1); //malloced one byte
			mallocA++;
			free(ptr); //freed one byte
			freeA++;
		}
		gettimeofday(&end, 0);
		average+= (end.tv_sec - start.tv_sec) * 1000000 + end.tv_usec - start.tv_usec;
	}
	average = average / 100;
	printf("\nTest A:\n");
	printf("Total number of mallocs: %d\n", mallocA);
	printf("Total number of frees: %d\n", freeA);
	printf("Average running time (seconds): %f\n", (float)average/1000000);
}

void testB(){ //store 150 bytes addresses in an array and then free them
	int *array[150];
	int mallocA=0;
	int success=0;
	int freeA=0;
	
	struct timeval start;
	struct timeval end;
	
        unsigned long average = 0;
	int i = 0, j = 0;

	for (i=0; i<100; i++) {
		gettimeofday(&start,0);
        	for (j=0; j<150; j++) { //Mallocs 150 times	
			array[j] =(int*)malloc(1); //pointers are stored in array
			mallocA++;
			success++;   
		}
       		for (j = 0; j < 150; j++) {
			free(array[j]); //freed from 0 - 149
			freeA++;
		}
       		gettimeofday(&end, 0);
		average += ((end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec-start.tv_usec));
    	}
	average/=100;
	printf("\nTest B:\n");
    	printf("Total number of mallocs: %d\n", mallocA);
    	printf("Successful mallocs: %d\n", success);
	printf("Total number of frees: %d\n", freeA);
	printf("Average running time (seconds): %f\n ", (float)average/1000000);
}	

void testC(){//Randomly choose between freeing or mallocing a 1 byte pointer
	int *array[150];
	int freeA = 0;
	int mallocA = 0;
	
	time_t b;
	srand((unsigned) time(&b));//random seed pool
	
	unsigned long average = 0;
	struct timeval start;
	struct timeval end;
	
	int i = 0, j = 0;
	
	for (i = 0; i < 100; i++){ //runs workload 100 times
	gettimeofday(&start, 0);
	int numFree = 0; //integers to keep track of pointer addresses in array just in case we malloc more than once.
	int numMalloc = 0;
	
	j = 0;
	while(j != 150) {
       
            int random = rand() % 2; //pick 0 or 1 to either malloc or free
            if (random == 1){ //1 = malloc
            	int *node = malloc(1);
            	array[numMalloc] = node;
		numMalloc++;
            	mallocA++;
		j++;
            }
            
            else if(random == 0){ //free
            	if (numMalloc > numFree){//in case we have more amount of times malloced then we do free, we check to see if the pointer was malloced before, or was it just an empty free call.
            		freeA++;
            		free(array[numFree]);
			numFree++;
            	}
            }
	      while(numFree < numMalloc) {//in the end, we have to free rest of pointers
            	freeA++;
            	free(array[numFree]);
		numFree++;
	      }
	    } 
	  gettimeofday(&end, 0);
	  average += (end.tv_sec-start.tv_sec)*1000000 + end.tv_usec-start.tv_usec;
	}
	average/=100;
	printf("\nTest C:\n");
	printf("Successful mallocs: %d\n", mallocA);
	printf("Total number of frees: %d\n", freeA);
	printf("Average runtime (seconds): %f\n ", (float)average/1000000);
}

void testD(){
  
    int *array[150];
    int freeA = 0;
    int mallocA = 0;
    long average = 0;
    
    struct timeval begin;
    struct timeval end;
    
    time_t b;
    srand((unsigned) time(&b));
    
    int x = 0, i = 0;
    
    for (x = 0; x < 100; x++){
       gettimeofday(&begin, 0);
       int numFree = 0;
       int numMalloc = 0;
       int totalMalloced = 0;
       i = 0;
       while (i != 150){
       
            int ranPick = rand() % 2; //pick 0 or 1
            if (ranPick == 1){ //1 = malloc
            	int ranNum = rand()%64 + 1; //randomnumber generator
		if (totalMalloced < 5000){
		  int* node =(int*)malloc(ranNum);
		  array[numMalloc] = node;
		  totalMalloced = totalMalloced + ranNum + 16;
		  numMalloc++;
		  mallocA++;
		}
		i++;
            }
            
            else { //free
            	if(numMalloc > numFree) {
            		free(array[numFree]);
			numFree++;
            		freeA++;
            	}
            }
            while(numFree < numMalloc) {
            	free(array[numFree]);
            	freeA++;
		numFree++;
            }
	} 
        gettimeofday(&end, 0);
        average += (end.tv_sec-begin.tv_sec)*1000000 + end.tv_usec-begin.tv_usec;
    }
  	average/=100;
	printf("\nTest D:\n");
	printf("Successful mallocs: %d\n", mallocA);
	printf("Total number of frees: %d\n", freeA);
	printf("Average runtime (seconds): %f\n ", (float)average/1000000);
}

void testE(){
    int *array[294];
    int mallocA = 0;
    int freeA = 0;
    
    unsigned long average = 0;
    
    struct timeval begin;
    struct timeval end;
    
    int i = 0, j = 0;
    for (j = 0; j < 100; j++){ //fill array and the memoryblock completely full
      gettimeofday(&begin, 0);
	for(i = 0; i < 294; i++){ 
	    int *ptr = (int*)malloc(1);
	    mallocA++;
	    array[i] = ptr;
	    if (i % 2 == 0){ //free every even integer
	      free(array[i]);
	      freeA++;
	    }
	} 
	for (i = 0; i < 294; i++){
	    gettimeofday(&begin, 0);
	    free(array[i]); //free rest of array with errors inbound.
	    freeA++;
	}
	gettimeofday(&end, 0);
	average += (end.tv_sec-begin.tv_sec)*1000000 + end.tv_usec-begin.tv_usec;
      }
      
      average /= 100;
      printf("\nTest E:\n");
      printf("Successful mallocs: %d\n", mallocA);
      printf("Total number of frees: %d\n", freeA); //free's is total amount of free's. NOT SUCCESSFULL FREES
      printf("Average runtime (seconds): %f\n ", (float)average/1000000);
}

void testF(){ //we test dead block's here.
    int *array[231];
    int mallocA = 0;
    int freeA = 0;
    
    unsigned long average = 0;
    
    struct timeval begin;
    struct timeval end;
    
    int j =0 , i =0, k=0;
    for (j = 0; j < 100; j++){
	gettimeofday(&begin,0);
      for (i = 0; i < 230; i++){ //fill memoryblock almost full
	  gettimeofday(&begin, 0);
	  int *ptr = (int*)malloc(1);
	  mallocA++;
	  array[i] = ptr;
	  gettimeofday(&end, 0);
      }
      int * ptr = (int*)malloc(1074); //fill last block to confirm space
      free(ptr); //free it
      int * ptr2 = (int*)malloc(300);
      ptr = (int*)malloc(300);
      int *tempptr = ptr;
      free(ptr2);
      ptr = (int*)malloc(442);//calls upon a split method that creates 3 new nodes. (300 + (16))->(300 + (16))->(442 + (16))
      free(ptr);
      ptr = (int*)malloc(443);//Although we have 600 free bytes, we can only allocate 442 or less.
      
      free(tempptr);
      freeA = freeA + 3;
      mallocA = mallocA + 3;      

      for(k = 0; k < 230; k++){
	free(array[k]);
	freeA++;
      }
      
      average += (end.tv_sec-begin.tv_sec)*1000000 + end.tv_usec-begin.tv_usec;
    }
    average /= 100;
    printf("\nTest F:\n");
    printf("Total amount of attempted mallocs: %d\n", mallocA);
    printf("Total number of frees: %d\n", freeA);
    printf("Average runtime (seconds): %f\n ", (float)average/1000000);
}


int main(int argc, char ** argv){
	testA();
	testB();
	testC();
	testD();
	testE();
	testF();
	return 0;
}
