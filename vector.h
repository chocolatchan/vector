#ifndef __VECTOR__
#define __VECTOR__

#include <stdlib.h>
#include <string.h>

// --- --- --- ---

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

typedef struct node node_t;

struct node {
    void   *data;
    node_t *next;
    node_t *prev;
};

node_t *node_init(void *__restrict__ data, size_t size, node_t *next, node_t *prev, void *(*cloner)(void*));
void node_destroy(node_t *node, void (*destroyer)(void*));

// --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- --- ---

typedef struct vector vector_t;

struct vector {
    node_t *head;
    node_t *tail;
    size_t  length;
    size_t  size;
    int     is_heap;
};

vector_t *vector_init_h(size_t size);
void vector_init_s(vector_t *vector, size_t size);
void vector_clear(vector_t *vector, void (*destroyer)(void*));
void vector_destroy(vector_t *vector, void (*destroyer)(void*));
void vector_append(vector_t *vector, void *__restrict__ data, void *(*cloner)(void*));
void vector_prepend(vector_t *vector, void *__restrict__ data, void *(*cloner)(void*));
void vector_insert(vector_t *vector, size_t index, void *__restrict__ data, void *(*cloner)(void*));
void vector_remove(vector_t *vector, size_t index, void (*destroyer)(void*));
void *vector_at(vector_t *vector, size_t index);

#endif