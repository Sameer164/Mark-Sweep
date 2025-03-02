//
// Created by Sameer on 3/1/25.
//

#include "stack.h"
#include <stdlib.h>

custom_stack_t* new_stack(size_t capacity) {
    custom_stack_t* stack = (custom_stack_t*)malloc(sizeof(custom_stack_t));
    if (stack == NULL) {
        return NULL;
    }

    stack->capacity = capacity;
    stack->count = 0;
    stack->data = calloc(capacity, sizeof(void*));
    if (stack->data == NULL) {
        stack_free(stack);
        return NULL;
    }
    return stack;
}

void stack_push(custom_stack_t * stack, void* obj) {
    if (stack->count == stack->capacity) {
        void** tmp = realloc(stack->data, 2 * stack->capacity * sizeof(void*)); // Doubling the capacity
        if (tmp == NULL) {
            return;
        }
        stack->capacity *= 2;
        stack->data = tmp;
    }
    stack->data[stack->count] = obj;
    stack->count++;
}

void* stack_pop(custom_stack_t * stack) {
    if (stack->count == 0) {
        return NULL;
    }
    stack->count--;
    return stack->data[stack->count];
}


void stack_free(custom_stack_t * stack) {
    if (stack == NULL) {
        return;
    }
    if (stack->data != NULL) {
        free(stack->data);
        stack->data = NULL; // Defensive to prevent dangling pointer issues
    }
    free(stack);
}