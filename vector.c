#include "vector.h"

#pragma region node

node_t *node_init(void *__restrict__ data, size_t size, node_t *next, node_t *prev, void *(*cloner)(void*)) {
    node_t *node = (node_t*) malloc(sizeof(node_t));
    if (node == NULL) {
        return NULL;
    }

    if (cloner != NULL) {
        void *cloned = cloner(data);
        if (cloned == NULL) {
            free(node);
            return NULL;
        }
        node->data = cloned;
    } else {
        node->data = malloc(size);
        if (node->data == NULL) {
            free(node);
            return NULL;
        }
        memcpy(node->data, data, size);
    }
    node->next = next;
    node->prev = prev;

    return node;
}

void node_destroy(node_t *node, void (*destroyer)(void*)) {
    if (node == NULL) {
        return;
    }
    if (destroyer != NULL) {
        destroyer(node->data);
    } else {
        free(node->data);
    }
    free(node);
}

#pragma endregion node

// --- --- --- --- --- --- --- --- --- --- --- --- ---

#pragma region vector

/**
 * @brief Initialize a vector on the heap with the given size.
 * @param size The size of the data in each node.
 * @return A pointer to the initialized vector.
 * @note The vector will be empty after this function is called.
 */
vector_t *vector_init_h(size_t size) {
    vector_t *vector = (vector_t*) malloc(sizeof(vector_t));
    if (vector == NULL) {
        return NULL;
    }

    vector->head    = NULL;
    vector->tail    = NULL;
    vector->size    = size;
    vector->length  = 0;
    vector->is_heap = 1;

    return vector;
}

/**
 * @brief Initialize a vector on stack with the given size.
 * @param vector The vector to initialize.
 * @param size The size of the data in each node.
 * @note The vector will be empty after this function is called.
 */
void vector_init_s(vector_t *vector, size_t size) {
    if (vector == NULL) {
        return;
    }
    vector->head    = NULL;
    vector->tail    = NULL;
    vector->size    = size;
    vector->length  = 0;
    vector->is_heap = 0;
}

/**
 * @brief Clear the vector and all its nodes without checking for NULL.
 * @param vector The vector to clear.
 * @param destroyer The function to call to destroy the data in each node.
 */
static void __vector_clear_all_unchk(vector_t *vector, void (*destroyer)(void*)) {
    node_t *current = vector->head;
    while (current != NULL) {
        node_t *next = current->next;
        node_destroy(current, destroyer);
        current = next;
    }
}

/**
 * @brief Clear the vector and all its nodes.
 * @param vector The vector to clear.
 * @param destroyer The function to call to destroy the data in each node.
 * @note If destroyer is NULL, the data in each node will not be destroyed.
 * @note If destroyer is not NULL, the data in each node will be destroyed using the destroyer function.
 * @note The vector will be empty after this function is called.
 */
void vector_clear(vector_t *vector, void (*destroyer)(void*)) {
    if (vector == NULL) {
        return;
    }
    __vector_clear_all_unchk(vector, destroyer);

    vector->head = NULL;
    vector->tail = NULL;
    vector->length = 0;
}

/**
 * @brief Destroy the vector and all its nodes.
 * @param vector The vector to destroy.
 * @param destroyer The function to call to destroy the data in each node.
 * @note If destroyer is NULL, the data in each node will not be destroyed.
 * @note If destroyer is not NULL, the data in each node will be destroyed using the destroyer function.
 * @note The vector will be NULL after this function is called.
 */
void vector_destroy(vector_t *vector, void (*destroyer)(void*)) {
    if (vector == NULL) {
        return;
    }
    __vector_clear_all_unchk(vector, destroyer);
    
    if (vector->is_heap) free(vector);
}

/**
 * @brief Append a new node to the vector.
 * @param vector The vector to append the node to.
 * @param data The data to insert into the node.
 * @param cloner The function to call to clone the data in the node.
 * @note If the vector is empty, the node will be prepended.
 */
void vector_append(vector_t *vector, void *__restrict__ data, void *(*cloner)(void*)) {
    if (vector == NULL) {
        return;
    }
    node_t *node = node_init(data, vector->size, NULL, vector->tail, cloner);
    if (node == NULL) {
        return;
    }
    if (vector->tail != NULL) {
        (vector->tail)->next = node;
        vector->tail = node;
    } else {
        vector->head = node;
        vector->tail = node;
    }
    vector->length++;
}

/**
 * @brief Prepend a new node to the vector.
 * @param vector The vector to prepend the node to.
 * @param data The data to insert into the node.
 * @param cloner The function to call to clone the data in the node.
 * @note If the vector is empty, the node will be appended.
 */
void vector_prepend(vector_t *vector, void *__restrict__ data, void *(*cloner)(void*)) {
    if (vector == NULL) {
        return;
    }
    node_t *node = node_init(data, vector->size, vector->head, NULL, cloner);
    if (node == NULL) {
        return;
    }
    if (vector->head != NULL) {
        (vector->head)->prev = node;
        vector->head = node;
    } else {
        vector->head = node;
        vector->tail = node;
    }
    vector->length++;
}

/**
 * @brief Insert a new node at the left of the given index.
 * @param vector The vector to insert the node into.
 * @param index The index to insert the node at.
 * @param data The data to insert into the node.
 * @note If the index is 0, the node will be prepended.
 * @note If the index is equal to the length of the vector, the node will be appended.
 * @note If the index is greater than the length of the vector, the node will not be inserted.
 */
void vector_insert(vector_t *vector, size_t index, void *__restrict__ data, void *(*cloner)(void*)) {
    if (vector == NULL) {
        return;
    }
    if (vector->length < index) {
        return;
    }
    if (index == 0) {
        vector_prepend(vector, data, cloner);
        return;
    }
    if (index == vector->length) {
        vector_append(vector, data, cloner);
        return;
    }
    node_t *current = vector->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    node_t *node = node_init(data, vector->size, current, current->prev, cloner);
    if (node == NULL) {
        return;
    }

    (current->prev)->next = node;
    current->prev = node;
    vector->length++;
}

#pragma endregion vector
