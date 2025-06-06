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

#pragma region basic

vector_t *vector_init_h(size_t size, void *(*cloner)(void*), void (*destroyer)(void*)) {
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

void vector_init_s(vector_t *__restrict__ vector, size_t size, void *(*cloner)(void*), void (*destroyer)(void*)) {
    if (vector == NULL) {
        return;
    }
    vector->head    = NULL;
    vector->tail    = NULL;
    vector->size    = size;
    vector->length  = 0;
    vector->is_heap = 0;
}

static void __vector_clear_all_unchk(vector_t *__restrict__ vector) {
    node_t *current = vector->head;
    while (current != NULL) {
        node_t *next = current->next;
        node_destroy(current, vector->destroyer);
        current = next;
    }
}

void vector_clear(vector_t *__restrict__ vector) {
    if (vector == NULL) {
        return;
    }
    __vector_clear_all_unchk(vector);

    vector->head = NULL;
    vector->tail = NULL;
    vector->length = 0;
}

void vector_destroy(vector_t *__restrict__ vector) {
    if (vector == NULL) {
        return;
    }
    __vector_clear_all_unchk(vector);
    
    if (vector->is_heap) free(vector);
}

void vector_append(vector_t *__restrict__ vector, void *__restrict__ data) {
    if (vector == NULL) {
        return;
    }
    node_t *node = node_init(data, vector->size, NULL, vector->tail, vector->cloner);
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

void vector_prepend(vector_t *__restrict__ vector, void *__restrict__ data) {
    if (vector == NULL) {
        return;
    }
    node_t *node = node_init(data, vector->size, vector->head, NULL, vector->cloner);
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

void vector_insert(vector_t *__restrict__ vector, size_t index, void *__restrict__ data) {
    if (vector == NULL) {
        return;
    }
    if (vector->length < index) {
        return;
    }
    if (index == 0) {
        vector_prepend(vector, data);
        return;
    }
    if (index == vector->length) {
        vector_append(vector, data);
        return;
    }
    node_t *current = __vector_at_unchk(vector, index);
    
    node_t *node = node_init(data, vector->size, current, current->prev, vector->cloner);
    if (node == NULL) {
        return;
    }

    (current->prev)->next = node;
    current->prev = node;
    vector->length++;
}

void vector_remove(vector_t *__restrict__ vector, size_t index) {
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
    
    node_destroy(current, vector->destroyer);
    vector->length--;
}

vector_t *vector_copy(vector_t vector) {
    if (&vector == NULL) {
        return NULL;
    }
    vector_t *result = vector_init_h(vector.size, vector.cloner, vector.destroyer);
    if (result == NULL) {
        return NULL;
    }
    node_t *current = vector.head;
    while (current != NULL) {
        vector_append(result, current->data);
        current = current->next;
    }
    return result;
}

void *vector_at(vector_t vector, size_t index) {
    if (&vector == NULL) {
        return NULL;
    }
    if (index >= vector.length) {
        return NULL;
    }
    node_t *current = __vector_at_unchk(&vector, index);

    return current->data;   
}

#pragma endregion basic

#pragma region functional

void vector_foreach(vector_t *__restrict__ vector, void (*action)(void*)) {
    if (vector == NULL) {
        return;
    }
    node_t *current = vector->head;
    while (current != NULL) {
        action(current->data);
        current = current->next;
    }
}

vector_t *vector_map(vector_t vector, void (*transform)(void*)) {
    if (&vector == NULL) {
        return NULL;
    }
    vector_t *result = vector_copy(vector);
    if (result == NULL) {
        return NULL;
    }
    node_t *current = result->head;
    while (current != NULL) {
        transform(current->data);
        current = current->next;
    }
    return result;
}

vector_t *vector_filter(vector_t vector, int (*predicate)(void*)) {
    if (&vector == NULL) {
        return NULL;
    }
    vector_t *result = vector_init_h(vector.size, vector.cloner, vector.destroyer);
    if (result == NULL) {
        return NULL;
    }
    node_t *current = vector.head;
    while (current != NULL) {
        if (predicate(current->data)) {
            vector_append(result, current->data);
        }
        current = current->next;
    }
    return result;
}

void *vector_reduce(vector_t vector, void (*accumulator)(void*, void*), void* default_value) {
    if (vector.head == NULL || default_value == NULL) {
        return NULL;
    }

    void *result;
    if (vector.cloner != NULL) {
        result = vector.cloner(default_value);
    } else {
        result = default_value;
    }
    node_t *current = vector.head;
    while (current != NULL) {
        accumulator(result, current->data);
        current = current->next;
    }
    return result;
}

#pragma endregion functional

#pragma region utilities

void vector_print(vector_t vector, void (*printer)(void*)) {
    if (&vector == NULL) {
        return;
    }
    node_t *current = vector.head;
    printf("[");
    while (current != NULL) {
        printer(current->data);
        printf("%s", current->next != NULL ? ", " : "");
        current = current->next;
    }
    printf("]\n");
}

void vector_reverse(vector_t *__restrict__ vector) {
    if (vector == NULL) {
        return;
    }
    node_t *current = vector->head;
    node_t *temp = NULL;
    while (current != NULL) {

                 temp = current->prev;
        current->prev = current->next;
        current->next = temp;

        current = current->prev;
    }
    if (temp != NULL) {
        vector->head = temp->prev;
    }
}

void vector_merge(vector_t *__restrict__ vector1, vector_t *vector2) {
    if (vector1 == NULL || &vector2 == NULL) {
        return;
    }
    vector_t *clone = vector_copy(*vector2);
    if (clone == NULL) {
        return;
    }
    vector1->tail->next = clone->head;
    clone->head->prev = vector1->tail;
    vector1->tail = clone->tail;
    vector1->length += clone->length;
    
    clone->head = NULL;
    clone->tail = NULL;
    vector_destroy(clone);
}

#pragma endregion utilities

#pragma endregion vector
