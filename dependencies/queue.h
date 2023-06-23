TQueue *InitQueue()
{
    TQueue *queue = (TQueue *)malloc(sizeof(TQueue));
    if (!queue)
        return NULL;

    queue->first = NULL;
    queue->last = NULL;

    return queue;
}

void AddQueue(TQueue *q, TTree *tree)
{
    TList list = (TList)malloc(sizeof(TCell));
    if (!list)
        return;

    list->info = *tree;
    list->next = NULL;

    if (q->last != NULL)
        q->last->next = list;
    else
        q->first = list;

    q->last = list;
}

TTree ExtractQueue(TQueue *q)
{
    if (q->first == NULL && q->last == NULL)
        return NULL;

    TList list = q->first;

    q->first = q->first->next;

    if (!q->first)
        q->last = NULL;

    TTree aux = list->info;
    free(list);

    return aux;
}

void DestroyQueue(TQueue **q)
{
    TList p, aux;
    p = (*q)->first;

    while (p)
    {
        aux = p;
        p = p->next;
        free(aux);
    }
    free(*q);
    *q = NULL;
}