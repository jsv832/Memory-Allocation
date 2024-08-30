#include "memory_management.h"

//initialise structure
struct block* linkedlistHead = 0;

void* _malloc(int size){
    
    //check if size is valid
    if (size <= 0){
        return 0;

    }

    //if size is valid start allocating heap to memory block
    if (linkedlistHead == 0){ 

        //make structure the heap's head
        //heap size is 4096
        //block is free to be used, has enough space.
        //size = size of memory block - head size
        //block is free and has just been initialised so it does not point at another memory block 

        linkedlistHead = (struct block*) sbrk(PAGE_SIZE);
        linkedlistHead->free = 1;
        linkedlistHead->size = (PAGE_SIZE) - sizeof(struct block);
        linkedlistHead->next = 0;
    }

    // in order to find a suitable memory block we create a new struct that will look through the heap
    // we want it to start at the heap's head so make it equal to previous struct
    struct block* currBlock = linkedlistHead;

    while(currBlock != 0){

        // check if memory block is free and has enough space left
        if(currBlock->free == 1 && currBlock->size >=(size + (sizeof(struct block)))){

            //if block found is usable then break out of loop 
            break;
        }
        // if memory block is unusable then we check the block the next pointer is pointing at
        currBlock = currBlock->next;
    }

    //loop through linked list
    if(currBlock == 0){
        
        //make new struct to go to next block in case 
        struct block* nextBlock = 0;
        nextBlock = (struct block*) sbrk(PAGE_SIZE);
        nextBlock->size = (PAGE_SIZE) - sizeof(struct block);
        nextBlock->next = 0; 
        nextBlock->free = 1;

        //we want it to start at the start of heap
        struct block *tmpBlock = linkedlistHead;

        //iterate through linked list until it finds pointer to next block
        while(tmpBlock->next != 0){ 

            tmpBlock = tmpBlock->next;
        }

        tmpBlock = nextBlock->next;

        //use block pointed at
        currBlock = nextBlock;
    }

    //if memory block is usable but doesn't have enough space
    //we split input into chunks that fit in usable memory blocks
    
    if(currBlock->size > (size + (sizeof(struct block)))){

        //get the next block and split the input between blocks
        struct block* nextBlock = 0;
        nextBlock = (struct block*) ((void *)currBlock + (sizeof(block)) + size);
        nextBlock->size = currBlock->size - ((sizeof(struct block) + size));
        nextBlock->next = currBlock->next; 
        nextBlock->free = 1;
        currBlock->size = size;
        currBlock->next = nextBlock;
    }
    //since memory had to be split it means block is full
    //so block is not free anymore
    currBlock->free = 0;
    printf("%d \n",sizeof(struct block));
    return (currBlock+1);
} 


 void _free(void *ptr){
    
    //check if pointer is valid
     if (ptr == 0){
         return;
     }

    //make new struct to make a memory block free
    //we want to free memory block so we make block free
    struct block* freeBlock = (struct block*) ptr - 1;
    freeBlock->free = 1;

    //make new struct to check block that are pointing at other blocks
    struct block* linkBlock = linkedlistHead;

    //iterate through block that is pointing at other blocks
    //add make memory blocks that are not full and have been used before to linkedlist
    //allowing us to reuse memory blocks that now have space that can be used to allocate memory
    while(linkBlock->next != 0){

        if(linkBlock->free == 1 && linkBlock->next != 0 && linkBlock->next->free == 1) {
            linkBlock->size = linkBlock->size + linkBlock->next->size;
            linkBlock->next = linkBlock->next->next;
        }

        else{
            linkBlock = linkBlock->next;
        }
    }
 }
