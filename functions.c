#include "declarations.h"

#define zero 0

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

    for (int i = startX; i < startX + size; i++)
        for (int j = startY; j < startY + size; j++)
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

    for (int i = startX; i < size; i++)
        for (int j = startY; j < size; j++)
        {
            mean += (avgColor.red - imageMatrix[i][j].red) * (avgColor.red - imageMatrix[i][j].red);
            mean += (avgColor.green - imageMatrix[i][j].green) * (avgColor.green - imageMatrix[i][j].green);
            mean += (avgColor.blue - imageMatrix[i][j].blue) * (avgColor.blue - imageMatrix[i][j].blue);
        }

    mean = mean / (3 * size * size);

    return mean;
}

// read the arg. when executing
void ReadExecArg(int argc, char *argv[])
{
    TTree arb = NULL;
    RGB **imageMatrix = NULL;
    unsigned int width = zero, height = zero, nodeMaxSize = zero;

    if(strcmp(argv[1],"-c1") == 0)
    {
        int similarity = atoi(argv[2]);

        ReadPPMfile(argv[3], &imageMatrix, &width, &height);
        CompressImage(&imageMatrix, &arb, width, zero, zero, similarity, &nodeMaxSize);
        WriteInfoTree(argv[4], arb, nodeMaxSize);
    }
    if(strcmp(argv[1],"-c2") == 0)
    {

    }
    if(strcmp(argv[1],"-d") == 0)
    {

    }

    
    //WriteCompressedFile("./quadtree.bin", arb, width);

    DestroyTree(&arb);
    DestroyImageMatrix(&imageMatrix, height);
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

    char line[4];
    unsigned int maxval;

    // read the header
    fgets(line, sizeof(line), file);
    line[strlen(line) - 1] = '\0'; // remove the '\n' from the end of the line

    if (strcmp(line, "P6") != 0)
    {
        printf("Error at reading the header\n");
        fclose(file);
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

    fclose(file);
}

// create the quadtree
void CompressImage(RGB ***imageMatrix, TTree *arb, unsigned int size, unsigned int startX, unsigned int startY, unsigned long long similarity, unsigned int *nodeMaxSize)
{
    if (size <= 0)
        return;

    RGB avgColor = AvgColor(*imageMatrix, size, startX, startY);
    unsigned long long mean = avgMean(*imageMatrix, avgColor, size, startX, startY);

    if (mean > similarity)
    {
        *arb = InitNode();

        CompressImage(imageMatrix, &(*arb)->topLeft, size / 2, startX, startY, similarity, nodeMaxSize);
        CompressImage(imageMatrix, &(*arb)->topRight, size / 2, startX, startY + size / 2, similarity, nodeMaxSize);
        CompressImage(imageMatrix, &(*arb)->botLeft, size / 2, startX + size / 2, startY, similarity, nodeMaxSize);
        CompressImage(imageMatrix, &(*arb)->botRight, size / 2, startX + size / 2, startY + size / 2, similarity, nodeMaxSize);
    }
    else
    {
        int length = size - startX;
        if (length > *nodeMaxSize)
            *nodeMaxSize = length;

        *arb = InitCNode(avgColor.red, avgColor.green, avgColor.blue);
        return;
    }
}

int max(int x, int y)
{
    return x > y ? x : y;
}

int CountLevel(TTree arb)
{
    int nrUL, nrUR, nrBL, nrBR;

    if (!arb)
        return 0;

    nrUL = CountLevel(arb->topLeft);
    nrUR = CountLevel(arb->topRight);
    nrBL = CountLevel(arb->botLeft);
    nrBR = CountLevel(arb->botRight);

    return 1 + max(max(nrUR, nrUL), max(nrBL, nrBR));
}

//counting the nodes which contains color
void CountCNodes(TTree arb, int *nr)
{
    if (!arb)
        return;

    if (arb->type == ColorNode)
        (*nr)++;

    CountCNodes(arb->topLeft, nr);
    CountCNodes(arb->topRight, nr);
    CountCNodes(arb->botLeft, nr);
    CountCNodes(arb->botRight, nr);
}

//creating a .out file with some info about the quadtree
void WriteInfoTree(char *fileName, TTree arb, unsigned int nodeMaxSize)
{
    FILE *file = fopen(fileName, "w");
    if (!file)
    {
        printf("Couldn't open the file\n");
        return;
    }

    int nr = 0;
    CountCNodes(arb, &nr);

    fprintf(file, "%d\n%d\n%d\n", CountLevel(arb), nr, nodeMaxSize);

    fclose(file);
}

//storing the values of the quadtree in a binary format
void WriteCompressedFile(char *fileName, TTree arb, unsigned int size)
{
    FILE *file = fopen(fileName, "wb");
    if(!file)
    {
        printf("Error at opening the file\n");
        return;
    }

    fwrite(&size, 1, sizeof(unsigned int), file);


    fclose(file);
}