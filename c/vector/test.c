#include <stdio.h>
#include "vector.h"

void add_one(int *data) {
    *data += 1;
}

void sum(int *x, int *y) {
    *x += *y;
}

int gt15(int *x) {
    return *x > 15;
}

void int_printer(int *data) {
    printf("%d", *data);
}

int main() {
    void (*printer)(void*) = (void (*)(void*)) int_printer;

    void (*add_one_mask)(void*) = (void (*)(void*)) add_one;

    void (*sum_mask)(void*, void*) = (void (*)(void*, void*)) sum;

    int (*gt15_mask)(void*) = (int (*)(void*)) gt15;

    printf("Testing vector.h\n");

    // Test 1: Initialize a vector and perform basic operations
    vector_t v;
    vector_init_s(&v, sizeof(int), NULL, NULL);

    int a_1 = 10, a_2 = 20, a_3 = 30;
    vector_append(&v, &a_1);
    vector_prepend(&v, &a_2);
    vector_append(&v, &a_3);

    printf("Vector size after append/prepend: %zu\n", v.length);
    vector_print(v, printer);

    // Test 2: Apply a function to all elements
    vector_foreach(&v, add_one_mask);
    printf("After applying add_one:\n");
    vector_print(v, printer);

    int zero = 0;
    printf("After applying sum: %d\n", *(int*) vector_reduce(v, sum_mask, &zero));

    vector_t *v_filtered = vector_filter(v, gt15_mask);
    printf("Vector after filtered with gt15:\n");
    vector_print(*v_filtered, printer);
    vector_destroy(v_filtered);

    vector_t cpy = *vector_copy(v);
    printf("Vector copy:\n");
    vector_print(cpy, printer);
    

    // Test 3: Remove elements
    // vector_remove(&v, 0);
    // printf("Vector size after removing index 0: %d\n", v.length);
    // printf("Vector[0]: %d\n", *(int*) vector_at(v, 0));

    // Test 4: Initialize a heap-allocated vector
    vector_t *v2 = vector_init_h(sizeof(int), NULL, NULL);
    int b = 50;
    vector_append(v2, &b);
    vector_prepend(v2, &a_1);
    printf("Heap vector size: %d\n", v2->length);
    vector_print(*v2, printer);

    printf("After merge:\n");
    vector_merge(v2, &v);
    vector_print(*v2, printer);

    // Cleanup
    vector_destroy(v2);
    vector_destroy(&v);

    return 0;
}