#ifndef __VECTOR__
#define __VECTOR__

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
    void   *(*cloner)(void*);
    void    (*destroyer)(void*);
};

/**
 * @brief Initialize a vector on the heap with the given size.
 * @param size The size of the data in each node.
 * @return A pointer to the initialized vector.
 * @note The vector will be empty after this function is called.
 */
vector_t *vector_init_h(size_t size, void *(*cloner)(void*), void (*destroyer)(void*));

/**
 * @brief Initialize a vector on stack with the given size.
 * @param vector The vector to initialize.
 * @param size The size of the data in each node.
 * @note The vector will be empty after this function is called.
 */
void vector_init_s(vector_t *vector, size_t size, void *(*cloner)(void*), void (*destroyer)(void*));

/**
 * @brief Clear the vector and all its nodes.
 * @param vector The vector to clear.
 * @param destroyer The function to call to destroy the data in each node.
 * @note If destroyer is NULL, the data in each node will not be destroyed.
 * @note If destroyer is not NULL, the data in each node will be destroyed using the destroyer function.
 * @note The vector will be empty after this function is called.
 */
void vector_clear(vector_t *vector);

/**
 * @brief Destroy the vector and all its nodes.
 * @param vector The vector to destroy.
 * @param destroyer The function to call to destroy the data in each node.
 * @note If destroyer is NULL, the data in each node will not be destroyed.
 * @note If destroyer is not NULL, the data in each node will be destroyed using the destroyer function.
 * @note The vector will be NULL after this function is called.
 */
void vector_destroy(vector_t *vector);

/**
 * @brief Append a new node to the vector.
 * @param vector The vector to append the node to.
 * @param data The data to insert into the node.
 * @param cloner The function to call to clone the data in the node.
 * @note If the vector is empty, the node will be prepended.
 */
void vector_append(vector_t *vector, void *__restrict__ data);

/**
 * @brief Prepend a new node to the vector.
 * @param vector The vector to prepend the node to.
 * @param data The data to insert into the node.
 * @param cloner The function to call to clone the data in the node.
 * @note If the vector is empty, the node will be appended.
 */
void vector_prepend(vector_t *vector, void *__restrict__ data);

/**
 * @brief Insert a new node at the left of the given index.
 * @param vector The vector to insert the node into.
 * @param index The index to insert the node at.
 * @param data The data to insert into the node.
 * @note If the index is 0, the node will be prepended.
 * @note If the index is equal to the length of the vector, the node will be appended.
 * @note If the index is greater than the length of the vector, the node will not be inserted.
 */
void vector_insert(vector_t *vector, size_t index, void *__restrict__ data);

/**
 * @brief Remove the node at the given index.
 * @param vector The vector to remove the node from.
 * @param index The index of the node to remove.
 * @param destroyer The function to call to destroy the data in the node.
 * @note If destroyer is NULL, the data in the node will not be destroyed.
 * @note If destroyer is not NULL, the data in the node will be destroyed using the destroyer function.
 */
void vector_remove(vector_t *vector, size_t index);

/**
 * @brief Get the node at the given index.
 * @param vector The vector to get the node from.
 * @param index The index of the node to get.
 * @return A pointer to the node at the given index.
 */
void *vector_at(vector_t vector, size_t index);

vector_t *vector_copy(vector_t vector);

/**
 * @brief Iterate over each node in the vector and apply the given action.
 * @param vector The vector to iterate over.
 * @param action The function to call for each node.
 * @note The action function will be called with the data in the node as its argument.
 */
void vector_foreach(vector_t *vector, void (*action)(void*));

/**
 * @brief Create a new vector by applying the given transform to each node in the vector.
 * @param vector The vector to transform.
 * @param transform The function to call for each node.
 * @note The transform function will be called with the data in the node as its argument.
 */
vector_t *vector_map(vector_t vector, void (*transform)(void*));

vector_t *vector_filter(vector_t vector, int (*predicate)(void*));

void *vector_reduce(vector_t vector, void (*accumulator)(void*, void*), void *default_value);

void vector_print(vector_t vector, void (*printer)(void*));


#endif