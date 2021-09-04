#include <cstring>

#include "Mem.h"

void *Heap::Malloc(size_t size) {
  FreeBlock *it = free_list_;
  FreeBlock **prev = &free_list_;

  while (it != nullptr) {
    // Size of mem block >= requested + length header
    if (it->length - sizeof(FreeBlock) >= size + sizeof(AllocBlock)) {
      // Get the location of the block to be allocated
      AllocBlock *alloc_header = reinterpret_cast<AllocBlock *>(it);

      // Get location of where a new free block is to be created
      void *next_header = reinterpret_cast<void *>(
          reinterpret_cast<char *>(alloc_header) + size + 8);

      // Points to section of memory after length header to be returned
      void *ret = reinterpret_cast<void *>(alloc_header + 1);

      // Copy the free block being allocated to after the alloc block
      FreeBlock *new_header = new (next_header) FreeBlock(*it);

      // Insert in the free list
      *prev = new_header;

      // Change the length to account for the allocated block
      new_header->length -= (size + 8);

      // Set the length of the Alloc Block
      alloc_header->length = size;

      return ret;
    }
    // prev = &it->next;
    if (it != (*prev) && it != (*prev)->next) prev = &(*prev)->next;
    it = it->next;
  }
  return nullptr;
}
