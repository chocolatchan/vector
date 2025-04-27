#include <stdio.h>
#include "vector.h"

void add_one(int *data) {
    *data += 1;
}



int main() {
    void (*add_one_mask)(void*) = (void (*)(void*)) add_one;
    // Example: Initialize a vector and test its functionality
    printf("Testing vector.h\n");

    // Add your vector initialization and test cases here
    // Example:
    vector_t v;
    vector_init_s(&v, sizeof(int), NULL, NULL);
    int a_1 = 10; int a_2 = 20;
    vector_append(&v, (void*) &a_1);
    vector_prepend(&v, (void*) &a_2);
    printf("Vector size: %d\n", v.length);
    printf("Vector[0]: %d\n", *(int*) vector_at(v, 0));
    vector_foreach(&v, add_one_mask);
    vector_remove(&v, 0);
    printf("Vector size: %d\n", v.length);
    printf("Vector[0]: %d\n", *(int*) vector_at(v, 0));

    vector_t *v2 = vector_init_h(sizeof(int), NULL, NULL);
    int b = 20;
    vector_append(v2, (void*) &b);
    printf("Vector size: %d\n", v2->length);
    vector_destroy(v2);
    vector_destroy(&v);

    return 0;
}