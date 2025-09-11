#include "int_queue.h"

int main(void)
{
    IntQueue *int_queue = int_queue_create(1);

    if (int_queue_push(int_queue, 1))
        printf("push 1\n");
    if (int_queue_push(int_queue, 2))
        printf("push 2\n");
    if (int_queue_push(int_queue, 3))
        printf("push 3\n");
    if (int_queue_push(int_queue, 4))
        printf("push 4\n");

    int value;
    if (int_queue_pop(int_queue, &value))
        printf("pop %i\n", value);
    if (int_queue_pop(int_queue, &value))
        printf("pop %i\n", value);
    if (int_queue_pop(int_queue, &value))
        printf("pop %i\n", value);
    if (int_queue_pop(int_queue, &value))
        printf("pop %i\n", value);

    int_queue_free(int_queue);

    return 0;
}
