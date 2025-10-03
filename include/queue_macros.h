/*
 * queue-macros
 * Copyright (C) 2025  Thomas George Hodgkinson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef QUEUE_MACROS_H
#define QUEUE_MACROS_H

#include <stdbool.h>
#include <stddef.h> // for size_t
#include <stdint.h> // for SIZE_MAX
#include <stdio.h>
#include <stdlib.h>

#define DECLARE_QUEUE_STRUCT(prefix, name) \
    typedef struct prefix##_queue name##Queue;

#define DECLARE_QUEUE_FUNCTIONS(prefix, name, type)                              \
    name##Queue *prefix##_queue_create(size_t initial_capacity);                 \
    void prefix##_queue_free(name##Queue *prefix##_queue);                       \
                                                                                 \
    const type *prefix##_queue_get_data(const name##Queue *prefix##_queue);      \
    type *prefix##_queue_get_data_mut(name##Queue *prefix##_queue);              \
    size_t prefix##_queue_get_count(const name##Queue *prefix##_queue);          \
    size_t prefix##_queue_get_capacity(const name##Queue *prefix##_queue);       \
    size_t prefix##_queue_get_head(const name##Queue *prefix##_queue);           \
    size_t prefix##_queue_get_tail(const name##Queue *prefix##_queue);           \
                                                                                 \
    bool prefix##_queue_push(name##Queue *prefix##_queue, type item);            \
    bool prefix##_queue_pop(name##Queue *prefix##_queue, type *out_item);        \
    bool prefix##_queue_peek(const name##Queue *prefix##_queue, type *out_item); \
    bool prefix##_queue_is_empty(const name##Queue *prefix##_queue);             \
    bool prefix##_queue_is_full(const name##Queue *prefix##_queue);              \
    void prefix##_queue_clear(name##Queue *prefix##_queue);

#define DEFINE_QUEUE_STRUCT(prefix, name, type) \
    typedef struct prefix##_queue               \
    {                                           \
        type *data;                             \
        size_t count;                           \
        size_t capacity;                        \
        size_t head;                            \
        size_t tail;                            \
    } name##Queue;

#define DEFINE_QUEUE_FUNCTIONS(prefix, name, type)                              \
    static inline bool prefix##_queue_grow_impl(name##Queue *prefix##_queue);   \
                                                                                \
    name##Queue *prefix##_queue_create(size_t initial_capacity)                 \
    {                                                                           \
        if (!initial_capacity)                                                  \
        {                                                                       \
            fprintf(                                                            \
                stderr,                                                         \
                "%s: Initial capacity cannot be 0\n",                           \
                __func__);                                                      \
            return NULL;                                                        \
        }                                                                       \
                                                                                \
        name##Queue *prefix##_queue = malloc(sizeof(*prefix##_queue));          \
        if (!prefix##_queue)                                                    \
        {                                                                       \
            perror("Allocation failure");                                       \
            return NULL;                                                        \
        }                                                                       \
                                                                                \
        if (initial_capacity > SIZE_MAX / sizeof(*prefix##_queue->data))        \
        {                                                                       \
            fprintf(                                                            \
                stderr,                                                         \
                "%s: Initial capacity cannot be greater than %zu\n",            \
                __func__,                                                       \
                SIZE_MAX / sizeof(*prefix##_queue->data));                      \
            free(prefix##_queue);                                               \
            return NULL;                                                        \
        }                                                                       \
                                                                                \
        prefix##_queue->count = 0;                                              \
        prefix##_queue->capacity = initial_capacity;                            \
        prefix##_queue->data = malloc(                                          \
            prefix##_queue->capacity * sizeof(*prefix##_queue->data));          \
        if (!prefix##_queue->data)                                              \
        {                                                                       \
            perror("Allocation failure");                                       \
            free(prefix##_queue);                                               \
            return NULL;                                                        \
        }                                                                       \
                                                                                \
        prefix##_queue->head = 0;                                               \
        prefix##_queue->tail = 0;                                               \
                                                                                \
        return prefix##_queue;                                                  \
    }                                                                           \
                                                                                \
    void prefix##_queue_free(name##Queue *prefix##_queue)                       \
    {                                                                           \
        if (!prefix##_queue || !prefix##_queue->data)                           \
            return;                                                             \
        free(prefix##_queue->data);                                             \
        free(prefix##_queue);                                                   \
    }                                                                           \
                                                                                \
    const type *prefix##_queue_get_data(const name##Queue *prefix##_queue)      \
    {                                                                           \
        return prefix##_queue->data;                                            \
    }                                                                           \
                                                                                \
    type *prefix##_queue_get_data_mut(name##Queue *prefix##_queue)              \
    {                                                                           \
        return prefix##_queue->data;                                            \
    }                                                                           \
                                                                                \
    size_t prefix##_queue_get_count(const name##Queue *prefix##_queue)          \
    {                                                                           \
        return prefix##_queue->count;                                           \
    }                                                                           \
                                                                                \
    size_t prefix##_queue_get_capacity(const name##Queue *prefix##_queue)       \
    {                                                                           \
        return prefix##_queue->capacity;                                        \
    }                                                                           \
                                                                                \
    size_t prefix##_queue_get_head(const name##Queue *prefix##_queue)           \
    {                                                                           \
        return prefix##_queue->head;                                            \
    }                                                                           \
                                                                                \
    size_t prefix##_queue_get_tail(const name##Queue *prefix##_queue)           \
    {                                                                           \
        return prefix##_queue->tail;                                            \
    }                                                                           \
                                                                                \
    bool prefix##_queue_push(name##Queue *prefix##_queue, type item)            \
    {                                                                           \
        if (!prefix##_queue_grow_impl(prefix##_queue))                          \
            return false;                                                       \
        prefix##_queue->data[prefix##_queue->tail++] = item;                    \
        prefix##_queue->tail %= prefix##_queue->capacity;                       \
        prefix##_queue->count++;                                                \
        return true;                                                            \
    }                                                                           \
                                                                                \
    bool prefix##_queue_pop(name##Queue *prefix##_queue, type *out_item)        \
    {                                                                           \
        if (!prefix##_queue->count)                                             \
        {                                                                       \
            fprintf(stderr, "%s: Cannot pop, queue is empty\n", __func__);      \
            return false;                                                       \
        }                                                                       \
        *out_item = prefix##_queue->data[prefix##_queue->head++];               \
        prefix##_queue->head %= prefix##_queue->capacity;                       \
        prefix##_queue->count--;                                                \
        return true;                                                            \
    }                                                                           \
                                                                                \
    bool prefix##_queue_peek(const name##Queue *prefix##_queue, type *out_item) \
    {                                                                           \
        if (!prefix##_queue->count)                                             \
        {                                                                       \
            fprintf(stderr, "%s: Cannot peek, queue is empty\n", __func__);     \
            return false;                                                       \
        }                                                                       \
                                                                                \
        *out_item = prefix##_queue->data[prefix##_queue->head];                 \
        return true;                                                            \
    }                                                                           \
                                                                                \
    bool prefix##_queue_is_empty(const name##Queue *prefix##_queue)             \
    {                                                                           \
        return !prefix##_queue->count;                                          \
    }                                                                           \
    bool prefix##_queue_is_full(const name##Queue *prefix##_queue)              \
    {                                                                           \
        return prefix##_queue->count == prefix##_queue->capacity;               \
    }                                                                           \
                                                                                \
    void prefix##_queue_clear(name##Queue *prefix##_queue)                      \
    {                                                                           \
        prefix##_queue->count = 0;                                              \
        prefix##_queue->head = 0;                                               \
        prefix##_queue->tail = 0;                                               \
    }                                                                           \
                                                                                \
    static inline bool prefix##_queue_grow_impl(name##Queue *prefix##_queue)    \
    {                                                                           \
        if (prefix##_queue->count < prefix##_queue->capacity)                   \
            return true;                                                        \
                                                                                \
        if (prefix##_queue->capacity > SIZE_MAX / 2)                            \
        {                                                                       \
            fprintf(                                                            \
                stderr,                                                         \
                "%s: capacity (%zu) cannot be doubled without overflow. "       \
                "Max allowed is %zu entries.\n",                                \
                __func__,                                                       \
                prefix##_queue->capacity,                                       \
                SIZE_MAX / 2);                                                  \
            return false;                                                       \
        }                                                                       \
                                                                                \
        const size_t new_capacity = prefix##_queue->capacity * 2;               \
                                                                                \
        if (new_capacity > SIZE_MAX / sizeof(*prefix##_queue->data))            \
        {                                                                       \
            fprintf(                                                            \
                stderr,                                                         \
                "%s: increasing capacity would overflow malloc. "               \
                "Max capacity in bytes is %zu\n",                               \
                __func__,                                                       \
                SIZE_MAX / sizeof(*prefix##_queue->data));                      \
            return false;                                                       \
        }                                                                       \
                                                                                \
        type *new_data = malloc(new_capacity * sizeof(*prefix##_queue->data));  \
        if (!new_data)                                                          \
        {                                                                       \
            perror("Allocation failure");                                       \
            return false;                                                       \
        }                                                                       \
                                                                                \
        type *old_data = prefix##_queue->data;                                  \
        const size_t old_count = prefix##_queue->count;                         \
        const size_t old_capacity = prefix##_queue->capacity;                   \
        size_t old_head = prefix##_queue->head;                                 \
        const size_t old_tail = prefix##_queue->tail;                           \
                                                                                \
        prefix##_queue->data = new_data;                                        \
        prefix##_queue->capacity = new_capacity;                                \
        prefix##_queue->count = 0;                                              \
        prefix##_queue->head = 0;                                               \
        prefix##_queue->tail = 0;                                               \
                                                                                \
        for (size_t i = 0; i < old_count; ++i)                                  \
        {                                                                       \
            type item = old_data[old_head++];                                   \
            old_head %= old_capacity;                                           \
            if (!prefix##_queue_push(prefix##_queue, item))                     \
            {                                                                   \
                free(old_data);                                                 \
                return false;                                                   \
            }                                                                   \
        }                                                                       \
                                                                                \
        free(old_data);                                                         \
        return true;                                                            \
    }

#endif // QUEUE_MACROS_H
