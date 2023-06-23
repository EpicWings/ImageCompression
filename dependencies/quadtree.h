// initialize a color node
TTree InitCNode(unsigned char red, unsigned char green, unsigned char blue)
{
    TTree aux = (TTree)malloc(sizeof(TNode));
    if (!aux)
        return NULL;

    aux->type = ColorNode;

    aux->info = (RGB *)malloc(sizeof(RGB));
    if (!aux->info)
        return NULL;

    ((RGB *)aux->info)->red = red;
    ((RGB *)aux->info)->green = green;
    ((RGB *)aux->info)->blue = blue;

    aux->topLeft = aux->topRight = aux->botLeft = aux->botRight = NULL;

    return aux;
}

// initialize an empty node
TTree InitNode()
{
    TTree aux = (TTree)malloc(sizeof(TNode));
    if (!aux)
        return NULL;

    aux->type = EmptyNode;
    aux->info = NULL;
    aux->topLeft = aux->topRight = aux->botLeft = aux->botRight = NULL;

    return aux;
}

void DestroyNode(TTree arb)
{
    if (!arb)
        return;

    DestroyNode(arb->topLeft);
    DestroyNode(arb->topRight);
    DestroyNode(arb->botLeft);
    DestroyNode(arb->botRight);

    if (arb->type == ColorNode)
        free(arb->info);

    free(arb);
}

void DestroyTree(TTree *arb)
{
    if (!(*arb))
        return;
    DestroyNode(*arb);
    *arb = NULL;
}

void PrintQuadTree(TTree arb)
{
    if (!arb)
    {
        return;
    }

    if (arb->type == ColorNode)
    {
        printf("(%d %d %d)", ((RGB *)arb->info)->red, ((RGB *)arb->info)->green, ((RGB *)arb->info)->blue);
        return;
    }

    printf("[");
    printf("0 ");
    PrintQuadTree(arb->topLeft);
    PrintQuadTree(arb->topRight);
    PrintQuadTree(arb->botLeft);
    PrintQuadTree(arb->botRight);

    printf("]");
}