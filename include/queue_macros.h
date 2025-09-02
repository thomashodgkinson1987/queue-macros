#ifndef QUEUE_MACROS_H
#define QUEUE_MACROS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DECLARE_QUEUE_STRUCT(prefix, name, type) \
    typedef struct prefix##_queue                \
    {                                            \
        type *data;                              \
        size_t count;                            \
        size_t capacity;                         \
        size_t head;                             \
        size_t tail;                             \
    } name##Queue;

#define DECLARE_QUEUE_FUNCTIONS(prefix, name, type)                         \
    name##Queue prefix##_queue_create(size_t initial_capacity);             \
    void prefix##_queue_free(name##Queue *prefix##_queue);                  \
                                                                            \
    const type *prefix##_queue_get_data(const name##Queue *prefix##_queue); \
    type *prefix##_queue_get_data_mut(name##Queue *prefix##_queue);         \
    size_t prefix##_queue_get_count(const name##Queue *prefix##_queue);     \
    size_t prefix##_queue_get_capacity(const name##Queue *prefix##_queue);  \
    size_t prefix##_queue_get_head(const name##Queue *prefix##_queue);      \
    size_t prefix##_queue_get_tail(const name##Queue *prefix##_queue);      \
                                                                            \
    void prefix##_queue_enqueue(name##Queue *prefix##_queue, type item);    \
    type prefix##_queue_dequeue(name##Queue *prefix##_queue);               \
    type prefix##_queue_peek(const name##Queue *prefix##_queue);            \
    bool prefix##_queue_is_empty(const name##Queue *prefix##_queue);        \
    bool prefix##_queue_is_full(const name##Queue *prefix##_queue);         \
    void prefix##_queue_clear(name##Queue *prefix##_queue);

#define DEFINE_QUEUE_FUNCTIONS(prefix, name, type)                             \
    static inline void prefix##_queue_grow_impl(name##Queue *prefix##_queue);  \
                                                                               \
    name##Queue prefix##_queue_create(size_t initial_capacity)                 \
    {                                                                          \
        if (!initial_capacity)                                                 \
        {                                                                      \
            fprintf(                                                           \
                stderr,                                                        \
                "%s: Initial capacity cannot be 0\n",                          \
                __func__);                                                     \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
                                                                               \
        name##Queue prefix##_queue;                                            \
                                                                               \
        if (initial_capacity > SIZE_MAX / sizeof(*prefix##_queue.data))        \
        {                                                                      \
            fprintf(                                                           \
                stderr,                                                        \
                "%s: Initial capacity cannot be greater than %zu\n",           \
                __func__,                                                      \
                SIZE_MAX / sizeof(*prefix##_queue.data));                      \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
        prefix##_queue.count = 0;                                              \
        prefix##_queue.capacity = initial_capacity;                            \
        prefix##_queue.data = malloc(                                          \
            prefix##_queue.capacity * sizeof(*prefix##_queue.data));           \
                                                                               \
        if (!prefix##_queue.data)                                              \
        {                                                                      \
            perror("Allocation failure");                                      \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
                                                                               \
        prefix##_queue.head = 0;                                               \
        prefix##_queue.tail = 0;                                               \
                                                                               \
        return prefix##_queue;                                                 \
    }                                                                          \
                                                                               \
    void prefix##_queue_free(name##Queue *prefix##_queue)                      \
    {                                                                          \
        free(prefix##_queue->data);                                            \
        prefix##_queue->data = NULL;                                           \
        prefix##_queue->count = 0;                                             \
        prefix##_queue->capacity = 0;                                          \
        prefix##_queue->head = 0;                                              \
        prefix##_queue->tail = 0;                                              \
    }                                                                          \
                                                                               \
    const type *prefix##_queue_get_data(const name##Queue *prefix##_queue)     \
    {                                                                          \
        return prefix##_queue->data;                                           \
    }                                                                          \
                                                                               \
    type *prefix##_queue_get_data_mut(name##Queue *prefix##_queue)             \
    {                                                                          \
        return prefix##_queue->data;                                           \
    }                                                                          \
                                                                               \
    size_t prefix##_queue_get_count(const name##Queue *prefix##_queue)         \
    {                                                                          \
        return prefix##_queue->count;                                          \
    }                                                                          \
                                                                               \
    size_t prefix##_queue_get_capacity(const name##Queue *prefix##_queue)      \
    {                                                                          \
        return prefix##_queue->capacity;                                       \
    }                                                                          \
                                                                               \
    size_t prefix##_queue_get_head(const name##Queue *prefix##_queue)          \
    {                                                                          \
        return prefix##_queue->head;                                           \
    }                                                                          \
                                                                               \
    size_t prefix##_queue_get_tail(const name##Queue *prefix##_queue)          \
    {                                                                          \
        return prefix##_queue->tail;                                           \
    }                                                                          \
                                                                               \
    void prefix##_queue_enqueue(name##Queue *prefix##_queue, type item)        \
    {                                                                          \
        prefix##_queue_grow_impl(prefix##_queue);                              \
        prefix##_queue->data[prefix##_queue->tail++] = item;                   \
        prefix##_queue->tail %= prefix##_queue->capacity;                      \
        prefix##_queue->count++;                                               \
    }                                                                          \
                                                                               \
    type prefix##_queue_dequeue(name##Queue *prefix##_queue)                   \
    {                                                                          \
        if (!prefix##_queue->count)                                            \
        {                                                                      \
            fprintf(stderr, "%s: Cannot dequeue, queue is empty\n", __func__); \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
        type item = prefix##_queue->data[prefix##_queue->head++];              \
        prefix##_queue->head %= prefix##_queue->capacity;                      \
        prefix##_queue->count--;                                               \
        return item;                                                           \
    }                                                                          \
                                                                               \
    type prefix##_queue_peek(const name##Queue *prefix##_queue)                \
    {                                                                          \
        if (!prefix##_queue->count)                                            \
        {                                                                      \
            fprintf(stderr, "%s: Cannot peek, queue is empty\n", __func__);    \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
                                                                               \
        return prefix##_queue->data[prefix##_queue->head];                     \
    }                                                                          \
                                                                               \
    bool prefix##_queue_is_empty(const name##Queue *prefix##_queue)            \
    {                                                                          \
        return !prefix##_queue->count;                                         \
    }                                                                          \
    bool prefix##_queue_is_full(const name##Queue *prefix##_queue)             \
    {                                                                          \
        return prefix##_queue->count == prefix##_queue->capacity;              \
    }                                                                          \
                                                                               \
    void prefix##_queue_clear(name##Queue *prefix##_queue)                     \
    {                                                                          \
        prefix##_queue->count = 0;                                             \
        prefix##_queue->head = 0;                                              \
        prefix##_queue->tail = 0;                                              \
    }                                                                          \
                                                                               \
    static inline void prefix##_queue_grow_impl(name##Queue *prefix##_queue)   \
    {                                                                          \
        if (prefix##_queue->count < prefix##_queue->capacity)                  \
        {                                                                      \
            return;                                                            \
        }                                                                      \
                                                                               \
        if (prefix##_queue->capacity > SIZE_MAX / 2)                           \
        {                                                                      \
            fprintf(                                                           \
                stderr,                                                        \
                "%s: capacity (%zu) cannot be doubled without overflow. "      \
                "Max allowed is %zu entries.\n",                               \
                __func__,                                                      \
                prefix##_queue->capacity,                                      \
                SIZE_MAX / 2);                                                 \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
                                                                               \
        const size_t new_capacity = prefix##_queue->capacity * 2;              \
                                                                               \
        if (new_capacity > SIZE_MAX / sizeof(*prefix##_queue->data))           \
        {                                                                      \
            fprintf(                                                           \
                stderr,                                                        \
                "%s: increasing capacity would overflow malloc. "              \
                "Max capacity in bytes is %zu\n",                              \
                __func__,                                                      \
                SIZE_MAX / sizeof(*prefix##_queue->data));                     \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
                                                                               \
        type *new_data = malloc(new_capacity * sizeof(*prefix##_queue->data)); \
        if (!new_data)                                                         \
        {                                                                      \
            perror("Allocation failure");                                      \
            exit(EXIT_FAILURE);                                                \
        }                                                                      \
                                                                               \
        type *old_data = prefix##_queue->data;                                 \
        const size_t old_count = prefix##_queue->count;                        \
        const size_t old_capacity = prefix##_queue->capacity;                  \
        size_t old_head = prefix##_queue->head;                                \
        const size_t old_tail = prefix##_queue->tail;                          \
                                                                               \
        prefix##_queue->data = new_data;                                       \
        prefix##_queue->capacity = new_capacity;                               \
        prefix##_queue->count = 0;                                             \
        prefix##_queue->head = 0;                                              \
        prefix##_queue->tail = 0;                                              \
                                                                               \
        for (size_t i = 0; i < old_count; ++i)                                 \
        {                                                                      \
            type item = old_data[old_head++];                                  \
            old_head %= old_capacity;                                          \
            prefix##_queue_enqueue(prefix##_queue, item);                      \
        }                                                                      \
                                                                               \
        free(old_data);                                                        \
    }

#endif // QUEUE_MACROS_H
