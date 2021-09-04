#include <cstring>
#include <iostream>

#include "Mem.h"

void Heap::Free(void *ptr) {
  if (ptr == nullptr) return;

  FreeBlock *it = free_list_;
  FreeBlock **prev = &free_list_;

  while (it != nullptr) {
    if (it > ptr) {
      // Get location of length value for Alloc Block
      char *pos = reinterpret_cast<char *>(ptr) - 8;

      // Store this value on the stack
      AllocBlock *allocd =
          new AllocBlock(*(reinterpret_cast<AllocBlock *>(pos)));

      // Make a new Free Block
      FreeBlock *new_header = new (reinterpret_cast<void *>(pos)) FreeBlock;

      // Check if the new free block is adjacent to one at a higher address
      char *adj_check_right = reinterpret_cast<char *>(ptr) + allocd->length;
      if (adj_check_right == reinterpret_cast<char *>(it)) {
        new_header->length = allocd->length + it->length + 8;
        new_header->next = it->next;
      } else {
        new_header->length = allocd->length + 8;
        new_header->next = it;
      }

      // Check if new free block is adjacent to one at a lower address
      char *adj_check_left = reinterpret_cast<char *>(*prev) + (*prev)->length;
      if (adj_check_left == reinterpret_cast<char *>(new_header)) {
        (*prev)->length += new_header->length;
        (*prev)->next = new_header->next;
      } else {
        // If this new Free Block is the lowest memory position it should be the
        // start of
        //  the Free List
        if ((*prev) > ptr) {
          (*prev) = new_header;
        } else {
          (*prev)->next = new_header;
        }
      }
    }
    if (it != (*prev) && it != (*prev)->next) prev = &(*prev)->next;
    it = it->next;
  }
}
