#ifndef MY_MALLOC_H
#define MY_MALLOC_H
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
typedef struct t_block block;
struct t_block{
  size_t size;
  block * next;
};
 

block * head=NULL;


__thread block * head_tls = NULL;


block * bf_find_block(size_t size);
size_t align_size(size_t size);
block *creat_block(size_t size);
block *split_block(block *curr,size_t size);
block * is_valid(void *address);
block * merge_block(block *curr);
void delete_free(block *to_free);
void add_to_free(block *to_free);
 
void ts_free_lock(void *ptr);
void *ts_malloc_lock(size_t size);


block *split_block_nolock(block *curr,size_t size);
block * merge_block_nolock(block *curr);
void delete_free_nolock(block *to_free);
void add_to_free_nolock(block *to_free);
block *creat_block_nolock(size_t size);
block * is_valid_nolock(void *address);

void ts_free_nolock(void *ptr);
void *ts_malloc_nolock(size_t size);

unsigned long get_data_segment_size(); //in bytes
unsigned long get_data_segment_free_space_size(); //in bytes
#endif
