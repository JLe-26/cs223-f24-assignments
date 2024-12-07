/*
* This program implements the function memstats to print information
* such as the total num of memory blocks allocated (in-use and free)
* as well as the amount of underutilizsed memory (%).
*
* Underutilized memory is the percent of memory that has been allocated
* by sbrk but not being used by the application.
*/

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>
#include "rand.h"

#define ROUNDS 3
#define BUFFER 5
#define LOOP 10

struct chunk {
  int size;
  int used;
  struct chunk *next;
};

void memstats(struct chunk* freelist, void* buffer[], int len) {
  int total_blocks, free_blocks, used_blocks = 0;
  int total_mem, free_mem, used_mem = 0;

  struct chunk *next = freelist;
  while (next != NULL) {
    total_blocks++;
    free_blocks++;
    total_mem += next->size;
    free_mem += next->size - next->used; // requested size - used memory = free memory
    next = next->next;
  }

  for (int i = 0; i < len; i++) {
    
    if (buffer[i] == NULL) {
      
      continue;
    }

    // Move ptr to the chunk header just before the allocated memory block (move by sizeof chunk bytes)
    struct chunk *cnk = (struct chunk*) ((char*)buffer[i] - sizeof(struct chunk));
    
    total_blocks++;

    if (cnk->used > 0) {
      used_blocks++;
      used_mem += cnk->used;
    } else {
      free_blocks++;
      free_mem += cnk->size;
    }
  }


  // Compute underutilized memory
  float under_mem = 0.0;
  if(total_mem > 0) {
    under_mem = (free_mem * 100.0) / total_mem;
  }
  
  // Print information
  printf("Total blocks: %d Free blocks: %d Used blocks: %d \n", total_blocks, free_blocks, used_blocks);
  printf("Total memory allocated: %d Free memory: %d Used memory: %d \n", total_mem, free_mem, used_mem);
  printf("Underutilized memory: %.2f\n", under_mem);
}

int main ( int argc, char* argv[]) {

  printf("Starting test..\n");

  srand(100);

  double timer;
  struct timeval tstart, tend;

  gettimeofday(&tstart, NULL);

  void *buffer[BUFFER];
  for (int i = 0; i < BUFFER; i++) {
    buffer[i] = NULL;
  }

  void *init = sbrk(0);
  void *current;
  printf("The initial top of the heap is %p.\n", init);
  for (int j = 0 ; j < ROUNDS; j++) {
    printf("---------------\n%d\n" , j);

    for (int i= 0 ; i < LOOP ; i++) {
      int index = rand() % BUFFER;
      if (buffer[index] != NULL) {
        free(buffer[index]);
        buffer[index] = NULL;
        printf("Freeing index %d\n", index);
      } 
      else {
        size_t size = (size_t) randExp(8, 4000); 
        int *memory = NULL;
        memory = malloc(size);

        if (memory == NULL) {
          fprintf(stderr, "malloc failed\n");
          return(1);
        } 
        *memory = 123;
        buffer[index] = memory;
        printf("Allocating %d bytes at index %d\n", (int) size, index);
      }
    }
    extern struct chunk* flist;
    current = sbrk(0);
    int allocated = current - init;
    init = current;

    printf("The new top of the heap is %p.\n", current);
    printf("Increased by %d (0x%x) bytes\n", allocated, allocated);
    memstats(flist, buffer, BUFFER);
  }

  for (int i = 0; i < BUFFER; i++) {
    free(buffer[i]); 
  }
  gettimeofday(&tend, NULL);
  timer = tend.tv_sec - tstart.tv_sec + (tend.tv_usec - tstart.tv_usec)/1.e6;
  printf("Time is %g\n", timer);

  return 0 ;
}
