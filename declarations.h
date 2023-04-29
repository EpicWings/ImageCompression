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
} TNode, *TTree;

// functions
TTree InitCNode(unsigned char red, unsigned char green, unsigned char blue);
TTree InitNode();
void DestroyNode(TTree arb);
void DestroyTree(TTree *arb);
void DestroyImageMatrix(RGB ***imageMatrix, unsigned int height);
void ReadPPMfile(char *filename, RGB ***imageMatrix, unsigned int *width, unsigned int *height);
void CompressImage(RGB ***imageMatrix, TTree *arb, unsigned int size, unsigned int startX, unsigned int startY, unsigned long long similarity, unsigned int *nodeMaxSize);
RGB AvgColor(RGB **imageMatrix, unsigned int size, unsigned int startX, unsigned int startY);
int max(int x, int y);
int CountLevel(TTree arb);
void CountCNodes(TTree arb, int *nr);
void WriteInfoTree(char *fileName, TTree arb, unsigned int nodeMaxSize);
void WriteCompressedFile(char *fileName, TTree arb);