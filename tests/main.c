#include <stdio.h>

#include "int_queue.h"

int main(void)
{
    IntQueue int_queue = int_queue_create(1);

    int_queue_enqueue(&int_queue, 1);
    printf("enqueue 1\n");
    int_queue_enqueue(&int_queue, 2);
    printf("enqueue 2\n");
    int_queue_enqueue(&int_queue, 3);
    printf("enqueue 3\n");
    int_queue_enqueue(&int_queue, 4);
    printf("enqueue 4\n");

    printf("dequeue %i\n", int_queue_dequeue(&int_queue));
    printf("dequeue %i\n", int_queue_dequeue(&int_queue));
    printf("dequeue %i\n", int_queue_dequeue(&int_queue));
    printf("dequeue %i\n", int_queue_dequeue(&int_queue));

    int_queue_free(&int_queue);

    return 0;
}
