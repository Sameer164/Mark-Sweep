//
// Created by Sameer on 3/1/25.
//

#ifndef _GC_FOR_C__STACK_H
#define _GC_FOR_C__STACK_H

#include <stddef.h>

typedef struct Stack {
    size_t count;
    size_t capacity;
    void** data;
} custom_stack_t;

custom_stack_t* new_stack(size_t capacity);
void stack_push(custom_stack_t* stack, void* obj);
void* stack_pop(custom_stack_t* stack);
void stack_free(custom_stack_t* stack);


#endif //_GC_FOR_C__STACK_H
