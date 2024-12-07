/*
* This program implements malloc and free based on a free list
* and a first-fit strategy. This implementation uses a header to keep
* track of the memory requested from sbrk and the amount of memory
* currently used in the chunk.
*
* @author: Jenny Le
* @version: December 6, 2024
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct chunk {
  int size;
  int used;
  struct chunk *next;
};
struct chunk *flist = NULL;

void *malloc (size_t size) {
  // TODO: Implement malloc with a free list (See reading for details)
  if (size == 0) {
    return NULL; 
  }
  struct chunk *next = flist;
  struct chunk *prev = NULL;
  
  while (next != NULL) {
    // Search flist for a suitable size of memory
    if (next->size >= size) { 
      next->used = size; // If a chunk from the flist is suitable for the memory request, it is marked as used

      if (prev != NULL) {
        prev->next = next->next;
      } else {
      flist = next->next; 
      }

      return (void*)(next + 1);
    } else {
      prev = next;
      next = next->next;
    }
  }

  // Use sbrk to allocate new memory
  void *memory = sbrk(size + sizeof(struct chunk));
  if (memory == (void *)-1) {
    return NULL;
  } else {
    struct chunk *cnk = (struct chunk*)memory;
    cnk->size = size;
    cnk->used = size; // Requested size
    return (void*)(cnk + 1);
  }
}

void free(void *memory) {
  // TODO: Implement malloc with a free list (See reading for details)
  // memory is a reference to memory that we have allocated
  if (memory != NULL) {
    // Jump back one chunk position
    // The next ptr of the freed block now points to what flist points to
    struct chunk *cnk = (struct chunk*) ((struct chunk*)memory - 1);
    cnk->next = flist;
    flist = cnk;
  }
  return;
}
