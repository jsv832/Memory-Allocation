#include "kernel/types.h"
#include "user/user.h"

void* _malloc(int size);
void _free(void *ptr);

typedef struct block{
  int size;
  struct block *next;
  int free;
} block;

#define PAGE_SIZE 4096