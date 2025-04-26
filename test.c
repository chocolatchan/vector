#include <stdio.h>
#include "vector.h"

int main() {
    // Example: Initialize a vector and test its functionality
    printf("Testing vector.h\n");

    // Add your vector initialization and test cases here
    // Example:
    vector_t v;
    vector_init_s(&v, sizeof(int));
    int a_1 = 10; int a_2 = 20;
    vector_append(&v, (void*) &a_1, NULL);
    printf("Vector size: %d\n", v.length);
    vector_prepend(&v, (void*) &a_2, NULL);
    printf("Vector size: %d\n", v.length);
    

    vector_t *v2 = vector_init_h(sizeof(int));
    int b = 20;
    vector_append(v2, (void*) &b, NULL);
    printf("Vector size: %d\n", v2->length);
    vector_destroy(v2, NULL);
    vector_destroy(&v, NULL);

    return 0;
}