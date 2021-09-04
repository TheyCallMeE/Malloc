#include <cstring>
#include <iostream>

#include "Mem.h"

int main(int argc, char *argv[]) {
  Heap *heap = new Heap();

  heap->PrintFreeList();

  void *allocd = heap->Malloc(150);

  memcpy(allocd, "Hello there", 13);

  std::cout << "malloc call" << std::endl;

  heap->PrintFreeList();

  void *allocd2 = heap->Calloc(2, 200);

  memcpy(allocd2, "General Kenobi", 15);

  std::cout << "calloc call" << std::endl;

  heap->PrintFreeList();

  heap->Free(allocd);

  std::cout << "free call" << std::endl;

  heap->PrintFreeList();

  allocd2 = heap->Realloc(allocd2, 500);

  std::cout << "realloc call" << std::endl;

  heap->PrintFreeList();

  heap->Free(allocd2);

  std::cout << "free call" << std::endl;

  heap->PrintFreeList();

  return 0;
}
