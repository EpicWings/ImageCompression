#include "declarations.h"

#define filepath "./Exemple/exemplul1.ppm"
#define zero 0

int main()
{
    TTree arb = NULL;
    RGB **imageMatrix = NULL;
    unsigned int width = zero, height = zero, nodeMaxSize = zero;

    ReadPPMfile(filepath, &imageMatrix, &width, &height);
    CompressImage(&imageMatrix, &arb, width, zero, zero, 100,&nodeMaxSize);

    WriteInfoTree("./quadtree.out",arb,nodeMaxSize);

    DestroyTree(&arb);
    DestroyImageMatrix(&imageMatrix, height);
    return 0;
}