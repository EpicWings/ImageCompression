#include "declarations.h"

// initialize a color node
TArb InitCNode(unsigned char red, unsigned char green, unsigned char blue)
{
    TArb aux = (TArb)malloc(sizeof(TNod));
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
TArb InitNode()
{
    TArb aux = (TArb)malloc(sizeof(TNod));
    if (!aux)
        return NULL;

    aux->type = EmptyNode;
    aux->info = NULL;
    aux->topLeft = aux->topRight = aux->botLeft = aux->botRight = NULL;

    return aux;
}

RGB **InitImageMatrix(unsigned int width, unsigned int height)
{
    RGB **imageMatrix = (RGB **)malloc(height * sizeof(RGB *));
    if (!imageMatrix)
        return NULL;

    for (int i = 0; i < height; i++)
    {
        imageMatrix[i] = (RGB *)malloc(width * sizeof(RGB));
        if (!imageMatrix[i])
            return NULL;
    }

    return imageMatrix;
}

void DestroyNode(TArb arb)
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

void DestroyArb(TArb *arb)
{
    if (!(*arb))
        return;
    DestroyNode(*arb);
    *arb = NULL;
}

void DestroyImageMatrix(RGB ***imageMatrix, unsigned int height)
{
    for (int i = 0; i < height; i++)
        free((*imageMatrix)[i]);
    free(*imageMatrix);
    *imageMatrix = NULL;
}

RGB AvgColor(RGB **imageMatrix, unsigned int size, unsigned int startX, unsigned int startY)
{
    RGB avgColor;
    unsigned long long red = 0, green = 0, blue = 0;

    for(int i = startX; i < size; i++)
        for(int j = startY; j < size; j++)
        {
            red += imageMatrix[i][j].red;
            green += imageMatrix[i][j].green;
            blue += imageMatrix[i][j].blue;
        }
    red = red / (size * size);
    green = green / (size * size);
    blue = blue / (size * size);

    avgColor.red = red;
    avgColor.green = green;
    avgColor.blue = blue;

    return avgColor;    
}

unsigned long long avgMean(RGB **imageMatrix, RGB avgColor, unsigned int size, unsigned int startX, unsigned int startY)
{
    unsigned long long mean = 0;

    for(int i = startX; i < size; i++)
        for(int j = startY; j < size; j++)
        {
            mean += (avgColor.red - imageMatrix[i][j].red) * (avgColor.red - imageMatrix[i][j].red);
            mean += (avgColor.green - imageMatrix[i][j].green) * (avgColor.green - imageMatrix[i][j].green);
            mean += (avgColor.blue - imageMatrix[i][j].blue) * (avgColor.blue - imageMatrix[i][j].blue);
        }

    mean = mean / ( 3 * size * size);

    return mean;
}

// read a PPM file and store it in a RGB matrix
void ReadPPMfile(char *filename, RGB ***imageMatrix, unsigned int *width, unsigned int *height)
{
    FILE *file = fopen(filename, "rb");
    if (!file)
    {
        printf("Error at opening the file\n");
        return;
    }

    char *line = (char *)malloc(100 * sizeof(char));
    if (!line)
        return;

    unsigned int maxval;

    // read the header
    fgets(line, 100, file);

    if (strcmp(line, "P6\n") != 0)
    {
        printf("Error at reading the header\n");
        return;
    }
    fscanf(file, "%u %u %u", width, height, &maxval);
    fseek(file, 1, SEEK_CUR);

    *imageMatrix = InitImageMatrix(*width, *height);

    // read the image
    for (int i = 0; i < *width; i++)
        for (int j = 0; j < *height; j++)
        {
            fread(&(*imageMatrix)[i][j].red, sizeof(unsigned char), 1, file);
            fread(&(*imageMatrix)[i][j].green, sizeof(unsigned char), 1, file);
            fread(&(*imageMatrix)[i][j].blue, sizeof(unsigned char), 1, file);
        }

    free(line);
    fclose(file);
}

// create the quadtree
void CompressImage(RGB ***imageMatrix, TArb *arb, unsigned int size, unsigned int startX, unsigned int startY, unsigned long long similarity)
{
    RGB avgColor = AvgColor(*imageMatrix, size, startX, startY);
    unsigned long long mean = avgMean(*imageMatrix, avgColor, size, startX, startY);

    printf("%d %d %d\n", avgColor.red, avgColor.green, avgColor.blue);
    printf("%llu\n", mean);


}