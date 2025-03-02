//
// Created by Sameer on 3/1/25.
//

#include "munit/munit.h"
#include <stdbool.h>
#include <string.h>
#include "objects.h"

void test_object_equal(object_t* obj1, object_t* obj2); //May not work as expected for structs that have pointers
void test_int_object();
void test_float_object();
void test_bool_object();
void test_string_object();
void test_array_object();

void test_length_int();
void test_length_float();
void test_length_bool();
void test_length_string();
void test_length_array();

void test_add_to_array();
void test_set_arr();
void test_get_arr();

void test_add_int();
void test_add_float();
void test_add_string();
void test_add_array();
void test_add_bool();


int main() {
    test_int_object();
    test_float_object();
    test_bool_object();
    test_string_object();
    test_array_object();

    test_length_int();
    test_length_float();
    test_length_bool();
    test_length_string();
    test_length_array();

    test_add_to_array();
    test_set_arr();
    test_get_arr();

    test_add_int();
    test_add_float();
    test_add_string();
    test_add_array();
    test_add_bool();
}

void test_object_equal(object_t* obj1, object_t* obj2) {
    munit_assert_memory_equal(sizeof(object_t), obj1, obj2);
}

void test_int_object() {
    object_t* obj = new_int(1);
    munit_assert_true(obj->kind == INTEGER);
    munit_assert_int(obj->data.v_int, ==, 1);
}

void test_float_object() {
    object_t* obj = new_float((float)1.0);
    munit_assert_true(obj->kind == FLOAT);
    munit_assert_int(obj->data.v_float, ==, 1.0);
}

void test_bool_object() {
    object_t* obj = new_bool(true);
    munit_assert_true(obj->kind == BOOL);
    munit_assert_true(obj->data.v_bool);

    obj = new_bool(false);
    munit_assert_true(obj->kind == BOOL);
    munit_assert_false(obj->data.v_bool);
}

void test_string_object() {
    char* test_str = "hellllllo";
    object_t* obj = new_string(test_str);
    munit_assert_true(obj->kind == STRING);
    munit_assert_true(strcmp(test_str, obj->data.v_string)==0);
}


void test_array_object() {
    object_t* obj = new_array(8);
    munit_assert_true(obj->kind == ARRAY);
    munit_assert_true(obj->data.v_array.capacity == 8);
    munit_assert_true(obj->data.v_array.count == 0);
    munit_assert_not_null(obj->data.v_array.elements);
}

void test_length_int() {
    object_t* obj = new_int(8);
    munit_assert_true(length(obj) == 1);
}

void test_length_float() {
    object_t* obj = new_float((float)8);
    munit_assert_true(length(obj) == 1);
}

void test_length_bool() {
    object_t* obj = new_bool(true);
    munit_assert_true(length(obj) == 1);

}

void test_length_string() {
    char* test_str = "HELLO";
    object_t* obj = new_string(test_str);
    munit_assert_true(length(obj) == 5);
}

void test_length_array() {
    object_t* obj = new_array(1);
    munit_assert_true(length(obj) == 0);

    object_t* int_obj = new_int(1);
    add_to_array(obj, int_obj);
    munit_assert_true(length(obj) == 1);
}

void test_add_to_array() {
    object_t* obj = new_array(1);
    object_t* int_obj = new_int(1);
    add_to_array(obj, int_obj);
    test_object_equal(int_obj, obj->data.v_array.elements[0]);

    object_t* int_obj2 = new_int(2);
    add_to_array(obj, int_obj2);
    test_object_equal(int_obj2, obj->data.v_array.elements[1]);
    munit_assert_true(obj->data.v_array.capacity == 2);
    munit_assert_true(obj->data.v_array.count == 2);

    object_t* int_obj3 = new_int(3);
    add_to_array(obj, int_obj3);
    test_object_equal(int_obj3, obj->data.v_array.elements[2]);
    munit_assert_true(obj->data.v_array.capacity == 4);
    munit_assert_true(obj->data.v_array.count == 3);


}

void test_set_arr() {
    object_t* obj = new_array(1);
    object_t* int_obj = new_int(1);
    add_to_array(obj, int_obj);
    object_t* int_obj2 = new_int(2);
    set_arr(obj, 0, int_obj2);
    test_object_equal(int_obj2, obj->data.v_array.elements[0]);
}

void test_get_arr() {
    object_t* obj = new_array(1);
    object_t* int_obj = new_int(1);
    add_to_array(obj, int_obj);
    test_object_equal(int_obj, get_arr(obj, 0));

}

void test_add_int() {
    object_t* int1 = new_int(1);
    object_t* int2 = new_int(2);
    object_t* result = new_int(3);
    test_object_equal(result, add(int1, int2));
}

void test_add_float() {
    object_t* float1 = new_float(1);
    object_t* float2 = new_float(2);
    object_t* result = new_float(3);
    test_object_equal(result, add(float1, float2));
}

void test_add_string() {
    object_t* str1 = new_string("Hello ");
    object_t* str2 = new_string("World!");
    object_t* result = add(str1, str2);

    munit_assert_true(strcmp("Hello World!", result->data.v_string)==0);
}

void test_add_array() {
    object_t* obj = new_array(1);
    object_t* int_obj = new_int(1);
    add_to_array(obj, int_obj);

    object_t* int_obj2 = new_int(2);
    add_to_array(obj, int_obj2);

    object_t* int_obj3 = new_int(3);
    add_to_array(obj, int_obj3);

    object_t* obj2 = new_array(1);
    add_to_array(obj2, int_obj);
    add_to_array(obj2, int_obj2);
    add_to_array(obj2, int_obj3);

    object_t* expected = new_array(3);
    object_t* int_obj4 = new_int(2);
    object_t* int_obj5 = new_int(4);
    object_t* int_obj6 = new_int(6);
    add_to_array(expected, int_obj4);
    add_to_array(expected, int_obj5);
    add_to_array(expected, int_obj6);

    object_t* result = add(obj, obj2);

    test_object_equal(get_arr(result, 0), get_arr(expected, 0));
    test_object_equal(get_arr(result, 1), get_arr(expected, 1));
    test_object_equal(get_arr(result, 2), get_arr(expected, 2));

}

void test_add_bool() {
    object_t* bool1 = new_bool(true);
    object_t* bool2 = new_bool(false);
    object_t* result = add(bool1, bool2);
    munit_assert_null(result);
}
