#include "declarations.h"

#define filepath "./Exemple/exemplul1.ppm"
#define zero 0

int main()
{
    TArb arb = NULL;
    RGB **imageMatrix = NULL;
    unsigned int width, height;

    ReadPPMfile(filepath, &imageMatrix, &width, &height);
    CompressImage(&imageMatrix, &arb, width, zero, zero, 100);

    DestroyArb(&arb);
    DestroyImageMatrix(&imageMatrix, height);
    return 0;
}