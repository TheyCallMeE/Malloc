#include <string.h>

#include "Mem.h"

void *Heap::Realloc(void *ptr, size_t size) {
  if (ptr == nullptr) return Malloc(size);

  if (size == 0) {
    Free(ptr);
    return nullptr;
  }

  FreeBlock *available{nullptr};
  bool no_adj = false;

  // Get a length of the alloc block called upon
  char *pos = reinterpret_cast<char *>(ptr) - 8;
  AllocBlock *curr_block = new AllocBlock(*reinterpret_cast<AllocBlock *>(pos));
  size_t to_steal = size - curr_block->length;

  char *alloc_end = reinterpret_cast<char *>(ptr) + curr_block->length;

  FreeBlock *it = free_list_;
  FreeBlock **prev = &free_list_;

  while (it != nullptr) {
    if (it > ptr) {
      // Check to see if an adjacent free block at a higher address can be a
      // part of
      //  the reallocation
      if (alloc_end == reinterpret_cast<char *>(it) && it->length >= to_steal) {
        // Make a new FreeBlock header at the end of size to be taken
        void *next_header = reinterpret_cast<void *>(alloc_end + to_steal);
        FreeBlock *new_header = new (next_header) FreeBlock(*it);
        new_header->length -= to_steal;
        reinterpret_cast<AllocBlock *>(pos)->length = size;

        // If new head of FreeList move prev
        if ((*prev) > ptr) {
          (*prev) = new_header;
        } else {
          (*prev)->next = new_header;
        }

        return reinterpret_cast<void *>(pos + 8);
      }

      // Not adjacent or the adjacent block does not work
      else {
        no_adj = true;
      }
    }

    // If a location can fit the requested amount of bytes
    if (it->length >= size ||
        (available != nullptr && available->length >= size)) {
      // Before an adjacent block is analyzed to see if reallocation is possible
      if (no_adj == false) {
        available = it;
      }

      // A block at some point either before or after the ptr can be used for
      //  reallocation and a memcpy
      else if (no_adj == true) {  // For understanding, could just be "else"

        // Call Malloc on bytes_requested
        void *new_ptr = Malloc(size);

        // Change the length of the current ptr
        reinterpret_cast<AllocBlock *>(pos)->length = size;

        // memcpy current ptr into Malloc'd location
        new_ptr = memcpy(new_ptr, reinterpret_cast<void *>(pos),
                         curr_block->length + 8);

        // Free the current ptr
        Free(reinterpret_cast<void *>(pos + 8));

        // return Malloc'd location
        return new_ptr;
      }
    }

    // Seeing if the free block adjacent at a lower address than the alloc block
    // can
    //  be consumed during reallocation
    char *free_end = reinterpret_cast<char *>(it) + it->length;
    if (free_end == pos) {
      if (it->length <= (curr_block->length + sizeof(AllocBlock)) &&
          it->length + (curr_block->length + sizeof(AllocBlock)) >= size) {
        reinterpret_cast<AllocBlock *>(pos)->length = size;

        // Move the alloc block to a lower address and free the bottom of it
        FreeBlock temp = *it;

        // Adjust the length of the new free block
        temp.length = temp.length + curr_block->length - size;

        // Before the memory is overridden with the memmove
        FreeBlock *next_header = it->next;

        // Shift the alloc block up into the free block after adjusting the free
        // list
        if ((*prev) == it) {  // First entry in freelist is being consumed
          *prev = next_header;
        } else {
          (*prev)->next = next_header;
        }
        void *new_ptr =
            memmove(reinterpret_cast<void *>(it), reinterpret_cast<void *>(pos),
                    curr_block->length + sizeof(AllocBlock));

        // Move the free block to the end of the alloc block if there is space
        //  for a new free block
        void *to_free = reinterpret_cast<void *>(alloc_end - temp.length);

        if (temp.length > sizeof(FreeBlock)) {
          // Make a spoof AllocBlock header so Free can work
          reinterpret_cast<AllocBlock *>(to_free)->length =
              temp.length - sizeof(AllocBlock);
          Free(reinterpret_cast<void *>(reinterpret_cast<char *>(to_free) + 8));
        } else {
          reinterpret_cast<AllocBlock *>(new_ptr)->length += temp.length;
        }

        return reinterpret_cast<void *>(reinterpret_cast<char *>(new_ptr) + 8);
      }
    }
    if (it != *prev && it != (*prev)->next) *prev = (*prev)->next;
    it = it->next;
  }
  return nullptr;
}
