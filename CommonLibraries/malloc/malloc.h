#ifndef __MALLOC_H
#define __MALLOC_H
#include "stm32f10x.h"

#define MEM_BANK_MIN_SIZE 8//ÄÜ·ÖÅäµÄÄÚ´æ´óÐ¡
#define MEM_SIZE (2*1024)//¹ÜÀíµÄÄÚ´æµÄ´óÐ¡
#define MMTS (MEM_SIZE/MEM_BANK_MIN_SIZE)//ÄÚ´æ¹ÜÀí±íµÄ´óÐ¡

void malloc_init(void);
void * my_malloc(unsigned int size);
unsigned char my_free(void *mem_pointer);

#endif













