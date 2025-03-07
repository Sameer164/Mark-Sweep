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
    for (int i = 0; i < gc->frames->count; i++) {
        frame_free(gc->frames->data[i]);
    }
    stack_free(gc->frames);
    // Remember that stack_free doesn't free the underlying data held by the stack.
    // It only frees the pointer (stack->data) that would have pointed to the storage where the actual data objects were located.
    // So before calling stack_free or gc_free, we need to make sure the objects are properly freed up.
    if (gc->objects != NULL) {
        for (int j = 0; j < gc->objects->count; j++) {
            free_object(gc->objects->data[j]);
        }
    }
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


void add_object_to_frame(frame_t* frame,  object_t* obj) {
    stack_push(frame->references, obj);
}

void mark(gc_t* gc) {
    if (gc == NULL) {
        return;
    }
    for (int i = 0; i < gc->frames->count; i++) {
        frame_t* frame = gc->frames->data[i];
        for (int j = 0; j < frame->references->count; j++) {
            object_t* obj = frame->references->data[j];
            obj->is_marked = true;
        }
    }
}

void trace(gc_t* gc) {
    custom_stack_t* gray_objects = new_stack(8);
    if (gray_objects == NULL) {
        return;
    }
    for (int i = 0; i < gc->objects->count; i ++) {
        object_t * obj = gc->objects->data[i];
        if (obj->is_marked == true) {
            stack_push(gray_objects, obj);
        }
    }
    while (gray_objects->count != 0) {
        object_t* obj = stack_pop(gray_objects);
        trace_blacken_object(gray_objects, obj);
    }
    stack_free(gray_objects);
}

void trace_blacken_object(custom_stack_t *gray_objects, object_t *obj) {
    switch (obj->kind) {
        case INTEGER:
        case FLOAT:
        case STRING:
            break;
        case ARRAY:
            for (int i = 0; i < obj->data.v_array.count; i++) {
                trace_mark_object(gray_objects, obj->data.v_array.elements[i]);
            }
            break;
        default:
            break;
    }
}

void trace_mark_object(custom_stack_t *gray_objects, object_t *obj) {
    if (obj == NULL || obj->is_marked==true){
        return;
    }
    obj->is_marked = true;
    stack_push(gray_objects, obj);
}


void vm_collect_garbage(gc_t *gc) {
    mark(gc);
    trace(gc);
    sweep(gc);
}

void sweep(gc_t* gc) {
    if (gc == NULL) {
        return;
    }
    for (int i = 0; i < gc->objects->count; i++) {
        object_t* obj = gc->objects->data[i];
        if (obj->is_marked==true) {
            obj->is_marked = false;
        } else {
            free_object(obj);
            gc->objects->data[i] = NULL;
        }
    }
}