//
// Created by Sameer on 3/1/25.
//
#include "objects.h"
#include "gc.h"
#include <stdlib.h>
#include <string.h>


int length(object_t* obj) {
    switch(obj->kind) {
        case INTEGER:
        case FLOAT:
        case BOOL:
            return 1;
        case STRING:
            return (int)strlen(obj->data.v_string);
        case ARRAY:
            return (int)obj->data.v_array.count;
        default:
            return -1;
    }
}

object_t* add(gc_t* gc, object_t *a, object_t *b) {
    if (a == NULL || b == NULL) {
        return NULL;
    }

    if (a->kind == INTEGER) {
        if (b -> kind == INTEGER) {
            return new_int(gc, a->data.v_int + b->data.v_int);
        } else if (b -> kind == FLOAT) {
            return new_float(gc, a->data.v_int + b->data.v_float);
        } else {
            return NULL;
        }
    }

    if (a->kind == FLOAT) {
        if (b -> kind == FLOAT) {
            return new_float(gc, a->data.v_float + b->data.v_float);
        } else if (b -> kind == INTEGER) {
            return new_float(gc, a->data.v_float + b->data.v_int);
        } else {
            return NULL;
        }
    }

    if (a->kind==STRING) {
        if (b->kind != STRING){
            return NULL;
        }
        size_t new_str_len = strlen(a->data.v_string) + strlen(b->data.v_string);
        char* string = (char*)calloc((new_str_len + 1), sizeof(char));
        strcat(string, a->data.v_string);
        strcat(string, b->data.v_string);
        object_t * new_str = new_string(gc, string);
        free(string);
        return new_str;
    }

    if (a->kind == ARRAY) {
        if (b->kind != ARRAY) {
            return NULL;
        }
        object_t* new_arr = new_array(gc, a->data.v_array.count + b->data.v_array.count);
        for (int i = 0; i < a->data.v_array.count; i++) {
            set_arr(new_arr, i, get_arr(a, i));
        }
        for (int i = 0; i < b->data.v_array.count; i++) {
            set_arr(new_arr, a->data.v_array.count + i, get_arr(b, i));
        }

        return new_arr;
    }

    return NULL;
}

object_t* new_int(gc_t* gc, int val) {
    object_t* int_obj = create_and_track(gc);
    if (int_obj == NULL) {
        return NULL;
    }
    int_obj->kind = INTEGER;
    int_obj->data.v_int = val;
    return int_obj;
}

object_t* new_float(gc_t* gc, float val) {
    object_t* float_obj = create_and_track(gc);
    if (float_obj == NULL) {
        return NULL;
    }
    float_obj->kind = FLOAT;
    float_obj->data.v_float = val;
    return float_obj;
}


object_t* new_bool(gc_t* gc, bool val) {
    object_t* bool_obj = create_and_track(gc);
    if (bool_obj == NULL) {
        return NULL;
    }
    bool_obj->kind = BOOL;
    bool_obj->data.v_bool = val;
    return bool_obj;
}

object_t* new_string(gc_t* gc, char* val) {
    object_t* string_obj = create_and_track(gc);
    if (string_obj == NULL) {
        return NULL;
    }
    string_obj->kind = STRING;

    /**
     * Since char* is a value on the stack and setting int_obj->data.v_string = val would copy the pointer, we might be dealing with a dangling pointer issue.
     * So we will create a new string on the heap.
    **/

    char* tmp = (char*)malloc(strlen(val) + 1); //Accounting for the null terminator
    if (tmp == NULL) {
        free(string_obj);
        return NULL;
    }
    strcpy(tmp, val);

    string_obj->data.v_string = tmp;
    return string_obj;
}

object_t* new_array(gc_t* gc, size_t capacity) {
    object_t* array_obj = create_and_track(gc);
    if (array_obj == NULL) {
        return NULL;
    }
    array_obj->kind = ARRAY;
    object_t** elements = calloc(capacity, sizeof(object_t*));
    if (elements == NULL) {
        free(array_obj);
        return NULL;
    }
    array_obj->data.v_array = (object_arr_t){.capacity = capacity, .count = 0, .elements = elements};
    return array_obj;
}

object_t* add_to_array(object_t* arr, object_t* obj) {
    if (arr == NULL || obj == NULL) {
        return NULL;
    }
    if (arr->data.v_array.count == arr->data.v_array.capacity) {
        object_t** tmp = realloc(arr->data.v_array.elements, 2 * arr->data.v_array.capacity);
        if (tmp == NULL) {
            return NULL;
        }
        arr->data.v_array.capacity *= 2;
        arr->data.v_array.elements = tmp;
    }
    arr->data.v_array.elements[arr->data.v_array.count] = obj;
    arr->data.v_array.count ++;
    return arr;
}


object_t* set_arr(object_t* arr, int index, object_t* obj) {
    if (arr == NULL || obj == NULL) {
        return NULL;
    }
    if (index >= arr->data.v_array.count) {
        return NULL;
    }
    arr->data.v_array.elements[index] = obj;
    return obj;
}


object_t* get_arr(object_t* arr, int index) {
    if (arr == NULL) {
        return NULL;
    }
    if (index >= arr->data.v_array.count) {
        return NULL;
    }
    return arr->data.v_array.elements[index];
}


object_t* create_and_track(gc_t* gc) {
 object_t* obj = (object_t*)calloc(1, sizeof(object_t));
 if (obj == NULL) {
     return NULL;
 }
 gc_track_object(obj, gc);
 obj->is_marked = true;
 return obj;
}

void free_object(object_t* obj) {
    switch (obj->kind) {
        case INTEGER:
        case FLOAT:
        case BOOL:
            break;
        case STRING:
            free(obj->data.v_string);
            break;
        case ARRAY:
            free(obj->data.v_array.elements); // Again, this only frees the pointer to the elements array.
            break;
    }
    free(obj);
}
