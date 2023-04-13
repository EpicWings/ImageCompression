#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// struct
enum NodeType
{
    ColorNode,
    EmptyNode
};

typedef struct nod
{
    enum NodeType type;
    void *info;
    struct nod *topLeft, *topRight, *botLeft, *botRight;
} TNod, *TArb;

//functions
TArb InitCNode(char color);
TArb InitNode();
void DestroyNode(TArb arb);
void DestroyArb(TArb *arb);