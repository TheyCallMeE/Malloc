#ifndef LIBC_MEMALLOCATION_MEM_H_
#define LIBC_MEMALLOCATION_MEM_H_

#include <stdint.h>
#include <sys/mman.h>

#include <iostream>

struct FreeBlock {
  int64_t length;
  FreeBlock *next;
};

struct AllocBlock {
  int64_t length;
};

class Heap {
 public:
  Heap() {
    heap_ = mmap(nullptr, 2016, PROT_EXEC | PROT_READ | PROT_WRITE,
                 MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    free_list_ = new (heap_) FreeBlock();
    free_list_->length = 2016;
    free_list_->next = nullptr;
  }
  ~Heap() { munmap(heap_, 2016); }

  void *Malloc(size_t size);
  void Free(void *ptr);
  void *Calloc(size_t nmemb, size_t size);
  void *Realloc(void *ptr, size_t size);
  void PrintFreeList() {
    FreeBlock *it = free_list_;
    while (it != nullptr) {
      std::cout << "Address: " << it << " Length: " << it->length << std::endl
                << std::endl;
      it = it->next;
    }
  }

 private:
  FreeBlock *free_list_;
  void *heap_;
};
#endif  // LIBC_MEMALLOCATION_MEM_H_
