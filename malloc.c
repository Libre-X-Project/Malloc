
#include "malloc.h"

#define BLOCK_SIZE 16 // Minimum block size for alignment

typedef struct Block {
    size_t size;
    struct Block *next;
} Block;

static Block *free_list = NULL;
static void *memory_start = NULL;
static size_t memory_size = 0;

void init_memory(void *start, size_t size) {
    memory_start = start;
    memory_size = size;
    
    free_list = (Block *)start;
    free_list->size = size - sizeof(Block);
    free_list->next = NULL;
}

void *malloc(size_t size) {
    if (size == 0) return NULL;

    // Align size to the nearest multiple of BLOCK_SIZE
    size = (size + BLOCK_SIZE - 1) & ~(BLOCK_SIZE - 1);

    Block *prev = NULL;
    Block *curr = free_list;

    while (curr != NULL) {
        if (curr->size >= size) {
            // Found a block that fits
            if (curr->size > size + sizeof(Block)) {
                // Split the block
                Block *new_block = (Block *)((char *)curr + sizeof(Block) + size);
                new_block->size = curr->size - size - sizeof(Block);
                new_block->next = curr->next;
                curr->size = size;
                curr->next = new_block;
            } else {
                // Use the whole block
                if (prev != NULL) {
                    prev->next = curr->next;
                } else {
                    free_list = curr->next;
                }
            }
            return (char *)curr + sizeof(Block);
        }
        prev = curr;
        curr = curr->next;
    }
    return NULL; // No suitable block found
}

void free(void *ptr) {
    if (ptr == NULL) return;

    Block *block = (Block *)((char *)ptr - sizeof(Block));
    Block *curr = free_list;
    Block *prev = NULL;

    // Find the correct position to insert the block into the free list
    while (curr != NULL && curr < block) {
        prev = curr;
        curr = curr->next;
    }

    if (prev == NULL) {
        // Insert at the beginning
        block->next = free_list;
        free_list = block;
    } else {
        // Insert in the middle or end
        prev->next = block;
        block->next = curr;
    }

    // Merge adjacent free blocks
    if (block->next != NULL && (char *)block + sizeof(Block) + block->size == (char *)block->next) {
        block->size += sizeof(Block) + block->next->size;
        block->next = block->next->next;
    }
    if (prev != NULL && (char *)prev + sizeof(Block) + prev->size == (char *)block) {
        prev->size += sizeof(Block) + block->size;
        prev->next = block->next;
    }
}

