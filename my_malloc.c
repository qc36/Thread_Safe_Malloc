#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include<pthread.h>
#include <stdbool.h>
#include "my_malloc.h"

pthread_mutex_t lock_sbrk;

void printfreeList(){
  block * b=head;
  printf("free in list:\n");
  while(b!=NULL){
    printf("free.size %d\n",(int)(b->size));
    b=b->next;
  }
} 

void * get_data(block * b){
  block *temp=b;
  temp++;
  return temp;
}

void *ts_malloc_lock(size_t size){
  pthread_mutex_lock(&lock_sbrk);
  size=align_size(size);
  block * new_block;
  block * min=NULL;
  block * prev_min=NULL;
  block *prev_curr=NULL;
  block *curr=head;
   while((curr!=NULL)){
     if(curr->size>size){
         if(min==NULL){
          prev_min=prev_curr;
          min=curr;
        }
        else{
            if(curr->size<min->size){
              prev_min=prev_curr;
              min=curr;
              }
        }
    }
    else if(curr->size==size){
      prev_min=prev_curr; 
      min=curr;
      break;
    }
    prev_curr=curr;
    curr=curr->next;
  }
  if(min==NULL){
    new_block=creat_block(size);
    if (new_block==NULL){
       pthread_mutex_unlock(&lock_sbrk);
      return NULL;
    }
  }
  else if((min->size)>(sizeof(block)+size)){
    new_block=split_block(min,size);
  }
  else {
    new_block=min;
    if(min!=head)
      prev_min->next=min->next;
    else
      head=min->next;
    min->next=NULL;
  }
  pthread_mutex_unlock(&lock_sbrk);
  return get_data(new_block);
}


size_t align_size(size_t size){
  if(size%8==0)
    return size;
  else{
    return ((size_t)(size/8)+1)*8;
    }
}

block *creat_block(size_t size){
  block *new_block=sbrk(0);
  if(sbrk(sizeof(block)+size)==(void *)-1)
    return NULL;
  new_block->size=size;
  new_block->next=NULL;
  return new_block;    
}

block *split_block(block *curr,size_t size){
  block *new_block=NULL;
    curr->size-=size+sizeof(block);
    new_block= (block *)((size_t)curr + curr->size + sizeof(block));
    new_block->size=size;
    new_block->next=NULL;
  return new_block;
}


block * merge_block(block *curr){
 if(((size_t)curr->next-curr->size-sizeof(block))==(size_t)curr){
    curr->size+=sizeof(block)+curr->next->size;
    block *curr_next=curr->next;
    curr->next=curr_next->next;
    curr_next->next=NULL;
  }
  return curr;
}

void add_to_free(block *to_free){
    block *curr=head;
    block *prev=NULL;
  if(head==NULL){
    head=to_free;
    to_free->next=NULL; 
  }
  else{
    while((curr!=NULL)&&(curr<to_free)){
      prev=curr;
      curr=curr->next;
    }
    if((curr!=NULL)&&(curr>to_free)){
      if(head==curr){
        head=to_free;
        prev=NULL;
      }
      else{
        prev->next=to_free;
      }
      to_free->next=curr;
    }
    if(curr==NULL)
    {
        prev->next=to_free;
        to_free->next=NULL;
    }
  }
  
  if(prev!=NULL){
      merge_block(prev); 
  }
  if(to_free->next!=NULL){
      merge_block(to_free); 
  } 
}


void ts_free_lock(void *ptr){
 pthread_mutex_lock(&lock_sbrk);
 if(!ptr){
    pthread_mutex_unlock(&lock_sbrk);
   return;
 }
    block *to_free=ptr;
    to_free--; 
    if(!to_free){
       pthread_mutex_unlock(&lock_sbrk);
      return;
    }
    add_to_free(to_free);
    pthread_mutex_unlock(&lock_sbrk);
}

block *creat_block_nolock(size_t size){
  pthread_mutex_lock(&lock_sbrk);
  block *new_block=sbrk(0);
  if(sbrk(sizeof(block)+size)==(void *)-1){
    pthread_mutex_unlock(&lock_sbrk);
    return NULL;
  }
  pthread_mutex_unlock(&lock_sbrk);
  new_block->size=size;
  new_block->next=NULL;
  return new_block;      
}

block *split_block_nolock(block *curr,size_t size){
  block *new_block=NULL;
    curr->size-=size+sizeof(block);
    new_block= (block *)((size_t)curr + curr->size + sizeof(block));
    new_block->size=size;
    new_block->next=NULL;
  return new_block;
}


block * merge_block_nolock(block *curr){
 if(((size_t)curr->next-curr->size-sizeof(block))==(size_t)curr){
    curr->size+=sizeof(block)+curr->next->size;
    block *curr_next=curr->next;
    curr->next=curr_next->next;
    curr_next->next=NULL;
  }
  return curr;
}

void add_to_free_nolock(block *to_free){
    block *curr=head_tls;
    block *prev=NULL;
  if(head_tls==NULL){
    head_tls=to_free;
    to_free->next=NULL; 
  }
  else{
    while((curr!=NULL)&&(curr<to_free)){
      prev=curr;
      curr=curr->next;
    }
    if((curr!=NULL)&&(curr>to_free)){
      if(head_tls==curr){
        head_tls=to_free;
        prev=NULL;
      }
      
      else{
        prev->next=to_free;
      }
      to_free->next=curr;
    }
    if(curr==NULL)
    {
        prev->next=to_free;
        to_free->next=NULL;
    }
  }
  
  if(prev!=NULL){
      to_free=merge_block_nolock(prev);
  }
  if(to_free->next!=NULL){
      merge_block_nolock(to_free); 
  } 
}
 

void *ts_malloc_nolock(size_t size){
  size=align_size(size);
  block * new_block;
  block * min=NULL;
  block * prev_min=NULL;
  block *prev_curr=NULL;
  block *curr=head_tls;
   while((curr!=NULL)){
   // if((curr->size>=size)&&(curr->is_used==0)){
     if(curr->size>size){
         if(min==NULL){
          prev_min=prev_curr;
          min=curr;
        }
        else{
            if(curr->size<min->size){
              prev_min=prev_curr;
              min=curr;
              }
        }
    }
    else if(curr->size==size){
      prev_min=prev_curr; 
      min=curr;
      break;
    }
    prev_curr=curr;
    curr=curr->next;
  }
  if(min==NULL){
    new_block=creat_block_nolock(size);
    if (new_block==NULL){
      return NULL;
    }
  }
  else if((min->size)>(sizeof(block)+size)){
    new_block=split_block_nolock(min,size);
  }
  else {
    new_block=min;
    if(min!=head_tls)
      prev_min->next=min->next;
    else
      head_tls=min->next;
    min->next=NULL;
  }
  return get_data(new_block);
}


void ts_free_nolock(void *ptr){
    if(!ptr) return;
    block *to_free=ptr;
    to_free--; 
    if(!to_free) 
      return;
    add_to_free_nolock(to_free);
}

unsigned long get_data_segment_size(){
    unsigned long data_size=0;
    block *curr=head;
    data_size=(unsigned long)(sbrk(0)-(void *)head);

    return data_size;
}
unsigned long get_data_segment_free_space_size(){
    unsigned long free_size=0;
   block *curr=head;
  while(curr){
        free_size+=(unsigned long)curr->size+sizeof(block);
    curr=curr->next;
  }
  return free_size;
}


