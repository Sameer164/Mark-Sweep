//
// Created by Sameer on 3/1/25.
//

#ifndef _GC_FOR_C__OBJECTS_H
#define _GC_FOR_C__OBJECTS_H

#include <stdbool.h>
#include "stack.h"

typedef struct GarbageCollector gc_t;
typedef struct Object object_t;

typedef enum ObjectKind {
    INTEGER,
    FLOAT,
    BOOL,
    STRING,
    ARRAY
} object_kind_t;

typedef struct ObjectArray {
    size_t count;
    size_t capacity;
    object_t** elements;
} object_arr_t;

typedef union ObjectData {
    int v_int;
    float v_float;
    bool v_bool;
    char* v_string;
    object_arr_t v_array;
} object_data_t;

typedef struct Object {
    object_kind_t kind;
    object_data_t data;
} object_t;


int length(object_t* obj);
object_t* add(gc_t* gc, object_t *a, object_t *b);
object_t* new_int(gc_t* gc, int val);
object_t* new_float(gc_t* gc, float val);
object_t* new_bool(gc_t* gc, bool val);
object_t* new_string(gc_t* gc, char* val);
object_t* new_array(gc_t* gc, size_t capacity);
object_t* add_to_array(object_t* arr, object_t* obj);
object_t* set_arr(object_t* arr, int index, object_t* obj);
object_t* get_arr(object_t* arr, int index);
object_t* create_and_track(gc_t* gc);

#endif //_GC_FOR_C__OBJECTS_H
