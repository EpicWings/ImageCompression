#include "declarations.h"

int main()
{
    TArb arb = NULL;

    arb = InitNode();


    DestroyArb(&arb);
    
    return 0;
}