#include <string.h>

#include <climits>

#include "Mem.h"

void* Heap::Calloc(size_t nmemb, size_t size) {
  if (nmemb == 0 || size == 0) return nullptr;

  // Checks if nmemb * size will result in an integer overflow
  if (std::max(size, nmemb) > UINT64_MAX / std::min(nmemb, size))
    return nullptr;

  size_t bytes = nmemb * size;

  void* ret = Malloc(bytes);

  if (ret == nullptr) return nullptr;

  // calloc sets the memory to 0's
  memset(ret, 0, bytes);

  return ret;
}
