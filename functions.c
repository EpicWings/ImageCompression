#include "declarations.h"

//initialize a color node
TArb InitCNode(char color)
{
    TArb aux = (TArb) malloc(sizeof(TNod));
    if(!aux)
        return NULL;

    aux->type = ColorNode;
    aux->info = (char *) malloc(sizeof(char));
    if(!aux->info)
        return NULL;
    
    (*(char *)aux->info) = color;
    aux->topLeft = aux->topRight = aux->botLeft = aux->botRight = NULL;

    return aux;
}

TArb InitNode()
{
    TArb aux = (TArb) malloc(sizeof(TNod));
    if(!aux)
        return NULL;

    aux->type = EmptyNode;
    aux->info = NULL;
    aux->topLeft = aux->topRight = aux->botLeft = aux->botRight = NULL;

    return aux;
}

void DestroyNode(TArb arb)
{
    if(!arb)
        return;
    
    DestroyNode(arb->topLeft);
    DestroyNode(arb->topRight);
    DestroyNode(arb->botLeft);
    DestroyNode(arb->botRight);

    if(arb->type == ColorNode)
        free(arb->info);

    free(arb);
}

void DestroyArb(TArb *arb)
{
    if(!(*arb))
        return;
    DestroyNode(*arb);
    *arb = NULL;
}