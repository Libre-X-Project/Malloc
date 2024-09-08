/*
Written by Paul-philipp
Rosenjart, Founder of the AHK.
*/



#ifndef MALLOC_H // Header
#define MALLOC_H // Header

#include <stddef.h> // size_t

void init_memory(void *start, size_t size);

void *malloc(size_t size);

void free(void *ptr);

#endif // MALLOC_H
