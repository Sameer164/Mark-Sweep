//
// Created by Sameer on 3/2/25.
//

#ifndef _GC_FOR_C__GC_H
#define _GC_FOR_C__GC_H
#include "stack.h"
#include "objects.h"

typedef struct GarbageCollector {
    custom_stack_t* frames; // We will push a frame type here
    custom_stack_t* objects; // We will push the actual objects here.
} gc_t;

typedef struct Frame {
    custom_stack_t* references;
} frame_t; // We will push the actual objects here.

gc_t* new_gc();
void gc_free(gc_t* gc);

void vm_frame_push(gc_t *gc, frame_t *frame);
frame_t *vm_new_frame(gc_t *gc);


void frame_free(frame_t *frame);
void gc_track_object(object_t* obj, gc_t* gc);

#endif //_GC_FOR_C__GC_H
