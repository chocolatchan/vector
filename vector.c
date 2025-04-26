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

static void __vector_clear_all_unchk(vector_t *vector, void (*destroyer)(void*)) {
    node_t *current = vector->head;
    while (current != NULL) {
        node_t *next = current->next;
        node_destroy(current, destroyer);
        current = next;
    }
}

void vector_clear(vector_t *vector, void (*destroyer)(void*)) {
    if (vector == NULL) {
        return;
    }
    __vector_clear_all_unchk(vector, destroyer);

    vector->head = NULL;
    vector->tail = NULL;
    vector->length = 0;
}

void vector_destroy(vector_t *vector, void (*destroyer)(void*)) {
    if (vector == NULL) {
        return;
    }
    __vector_clear_all_unchk(vector, destroyer);
    
    if (vector->is_heap) free(vector);
}

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

static node_t *__vector_at_unchk(vector_t *vector, size_t index) {
    node_t *current = vector->head;
    for (size_t i = 0; i < index; i++) {
        current = current->next;
    }
    return current;
}

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
    node_t *current = __vector_at_unchk(vector, index);
    
    node_t *node = node_init(data, vector->size, current, current->prev, cloner);
    if (node == NULL) {
        return;
    }

    (current->prev)->next = node;
    current->prev = node;
    vector->length++;
}

void vector_remove(vector_t *vector, size_t index, void (*destroyer)(void*)) {
    if (vector == NULL) {
        return;
    }
    if (index >= vector->length) {
        return;
    }
    node_t *current = __vector_at_unchk(vector, index);
    
    if (current->prev != NULL) {
        (current->prev)->next = current->next;
    } else {
        vector->head = current->next;
    }
    if (current->next != NULL) {
        (current->next)->prev = current->prev;
    } else {
        vector->tail = current->prev;
    }
    
    node_destroy(current, destroyer);
    vector->length--;
}

void *vector_at(vector_t *vector, size_t index) {
    if (vector == NULL) {
        return NULL;
    }
    if (index >= vector->length) {
        return NULL;
    }
    node_t *current = __vector_at_unchk(vector, index);

    return current->data;

}

#pragma endregion vector
