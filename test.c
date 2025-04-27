#include <stdio.h>
#include "vector.h"

void add_one(int *data) {
    *data += 1;
}

int sum(int *x, int *y) {
    *x += *y;
}

int main() {
    void (*add_one_mask)(void*) = (void (*)(void*)) add_one;

    void (*sum_mask)(void*, void*) = (void (*)(void*, void*)) sum;

    printf("Testing vector.h\n");

    // Test 1: Initialize a vector and perform basic operations
    vector_t v;
    vector_init_s(&v, sizeof(int), NULL, NULL);

    int a_1 = 10, a_2 = 20, a_3 = 30;
    vector_append(&v, &a_1);
    vector_prepend(&v, &a_2);
    vector_append(&v, &a_3);

    printf("Vector size after append/prepend: %d\n", v.length);
    printf("Vector[0]: %d\n", *(int*) vector_at(v, 0));
    printf("Vector[1]: %d\n", *(int*) vector_at(v, 1));
    printf("Vector[2]: %d\n", *(int*) vector_at(v, 2));

    // Test 2: Apply a function to all elements
    vector_foreach(&v, add_one_mask);
    printf("After applying add_one:\n");
    printf("Vector[0]: %d\n", *(int*) vector_at(v, 0));
    printf("Vector[1]: %d\n", *(int*) vector_at(v, 1));
    printf("Vector[2]: %d\n", *(int*) vector_at(v, 2));

    int zero = 0;
    printf("After applying sum: %d\n", *(int*) vector_reduce(v, sum_mask, &zero));

    // Test 3: Remove elements
    vector_remove(&v, 0);
    printf("Vector size after removing index 0: %d\n", v.length);
    printf("Vector[0]: %d\n", *(int*) vector_at(v, 0));

    // Test 4: Initialize a heap-allocated vector
    vector_t *v2 = vector_init_h(sizeof(int), NULL, NULL);
    int b = 50;
    vector_append(v2, &b);
    printf("Heap vector size: %d\n", v2->length);
    printf("Heap Vector[0]: %d\n", *(int*) vector_at(*v2, 0));

    // Cleanup
    vector_destroy(v2);
    vector_destroy(&v);

    return 0;
}