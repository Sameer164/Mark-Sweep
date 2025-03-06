//
// Created by Sameer on 3/2/25.
//

#include "gc.h"
#include "stack.h"
#include "objects.h"
#include <stdlib.h>

gc_t* new_gc() {
    gc_t* ptr = (gc_t*)malloc(sizeof(gc_t));
    if (ptr == NULL) {
        return NULL;
    }
    ptr->frames = new_stack(8);
    ptr->objects = new_stack(8);
    return ptr;
}

void gc_free(gc_t* gc) {
    if (gc == NULL) {
        return;
    }
    // Remember that stack_free doesn't free the underlying data held by the stack.
    // It only frees the pointer (stack->data) that would have pointed to the storage where the actual data objects were located.
    // So before calling stack_free or gc_free, we need to make sure the objects are properly freed up.
    stack_free(gc->frames);
    stack_free(gc->objects);
    free(gc);
}

void gc_frame_push(gc_t *gc, frame_t *frame) {
    if (gc == NULL || frame == NULL) {
        return;
    }
    stack_push(gc->frames, frame);
}

frame_t *gc_new_frame(gc_t *gc) {
    if (gc == NULL) {
        return NULL;
    }
    frame_t* new_frame = (frame_t*)malloc(sizeof(frame_t));
    if (new_frame == NULL) {
        return NULL;
    }
    new_frame->references = new_stack(8);
    stack_push(gc->frames, new_frame);
    return new_frame;
}

void gc_track_object(object_t* obj, gc_t* gc) {
    if (gc == NULL || obj == NULL) {
        return;
    }
    stack_push(gc->objects, obj);
}


void frame_free(frame_t *frame) {
    if (frame == NULL) {
        return;
    }
    stack_free(frame->references);
    free(frame);
}
