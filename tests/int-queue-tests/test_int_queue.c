#include <assert.h>
#include <stdio.h>

#include "int_queue.h"

int main(void) {
    printf("--- Running Integer Queue Tests ---\n");

    IntQueue* q = int_queue_create(4);
    assert(q != NULL);

    int_queue_free(q);

    printf("--- Integer Queue Tests Passed ---\n");

    return 0;
}
