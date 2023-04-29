#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// structs
enum NodeType
{
    ColorNode,
    EmptyNode
};

typedef struct
{
    unsigned char red, blue, green;
} RGB;

typedef struct nod
{
    enum NodeType type;
    void *info;
    struct nod *topLeft, *topRight, *botLeft, *botRight;
} TNod, *TArb;

// functions
TArb InitCNode(unsigned char red, unsigned char green, unsigned char blue);
TArb InitNode();
void DestroyNode(TArb arb);
void DestroyArb(TArb *arb);
void DestroyImageMatrix(RGB ***imageMatrix, unsigned int height);
void ReadPPMfile(char *filename, RGB ***imageMatrix, unsigned int *width, unsigned int *height);
void CompressImage(RGB ***imageMatrix, TArb *arb, unsigned int size, unsigned int startX, unsigned int startY, unsigned long long similarity);
RGB AvgColor(RGB **imageMatrix, unsigned int size, unsigned int startX, unsigned int startY);