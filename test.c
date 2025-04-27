#include <stdio.h>
#include "vector.h"

int main() {
    // Example: Initialize a vector and test its functionality
    printf("Testing vector.h\n");

    // Add your vector initialization and test cases here
    // Example:
    vector_t v;
    vector_init_s(&v, sizeof(int), NULL, NULL);
    int a_1 = 10; int a_2 = 20;
    vector_append(&v, (void*) &a_1);
    printf("Vector size: %d\n", v.length);
    vector_prepend(&v, (void*) &a_2);
    printf("Vector size: %d\n", v.length);
    printf("Vector[0]: %d\n", *(int*) vector_at(v, 0));
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