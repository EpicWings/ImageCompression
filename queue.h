#include <stdlib.h>

typedef struct cell {
    unsigned char color;
    struct cell *next;
} TCell, *TList;

typedef struct queue {
    TList first;
    TList last;
} TQueue;

TQueue *InitQueue()
{
    TQueue *queue = (TQueue *)malloc(sizeof(TQueue));
    if(!queue)
        return NULL;

    queue->first = NULL;
    queue->last = NULL;

    return queue;
}