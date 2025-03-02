//
// Created by Sameer on 3/2/25.
//

#include "munit/munit.h"
#include <stdbool.h>
#include <string.h>
#include "objects.h"
#include "stack.h"

void test_new_stack();
void test_stack_push();
void test_stack_pop();
void test_stack_free();
void test_object_equal(object_t* obj1, object_t* obj2);

int main() {
    test_new_stack();
    test_stack_push();
    test_stack_pop();
    test_stack_free();

    return 0;
}

void test_object_equal(object_t* obj1, object_t* obj2) {
    munit_assert_memory_equal(sizeof(object_t), obj1, obj2);
}

void test_new_stack() {
    custom_stack_t* stack = new_stack(8);
    munit_assert_true(stack->capacity == 8);
    munit_assert_true(stack->count == 0);
    munit_assert_not_null(stack->data);
}

void test_stack_push() {
    custom_stack_t* stack = new_stack(1);
    object_t* int_obj = new_int(1);
    stack_push(stack, int_obj);
    munit_assert_true(stack->capacity == 1);
    munit_assert_true(stack->count == 1);
    test_object_equal(int_obj, stack->data[0]);

    object_t* int_obj2 = new_int(2);
    stack_push(stack, int_obj2);
    test_object_equal(int_obj2, stack->data[1]);
    munit_assert_true(stack->capacity == 2);
    munit_assert_true(stack->count == 2);

    object_t* int_obj3 = new_int(3);
    stack_push(stack, int_obj3);
    test_object_equal(int_obj3, stack->data[2]);
    munit_assert_true(stack->capacity == 4);
    munit_assert_true(stack->count == 3);
}

void test_stack_pop() {
    custom_stack_t* stack = new_stack(1);
    void* obj = stack_pop(stack);
    munit_assert_null(obj);

    object_t* int_obj = new_int(1);
    stack_push(stack, int_obj);
    object_t* int_obj2 = new_int(2);
    stack_push(stack, int_obj2);
    object_t* int_obj3 = new_int(3);
    stack_push(stack, int_obj3);

    test_object_equal(int_obj3, stack_pop(stack));
    test_object_equal(int_obj2, stack_pop(stack));
    test_object_equal(int_obj, stack_pop(stack));

    munit_assert_true(stack->capacity == 4);
    munit_assert_true(stack->count == 0);

}