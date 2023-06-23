#include "declarations.h"

#define zero 0

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

TQueue *InitQueue()
{
    TQueue *queue = (TQueue *)malloc(sizeof(TQueue));
    if (!queue)
        return NULL;

    queue->first = NULL;
    queue->last = NULL;

    return queue;
}

void AddQueue(TQueue *q, TTree *tree)
{
    TList list = (TList)malloc(sizeof(TCell));
    if (!list)
        return;

    list->info = *tree;
    list->next = NULL;

    if (q->last != NULL)
        q->last->next = list;
    else
        q->first = list;

    q->last = list;
}

TTree ExtractQueue(TQueue *q)
{
    if (q->first == NULL && q->last == NULL)
        return NULL;

    TList list = q->first;

    q->first = q->first->next;

    if (!q->first)
        q->last = NULL;

    TTree aux = list->info;
    free(list);

    return aux;
}

void DestroyQueue(TQueue **q)
{
    TList p, aux;
    p = (*q)->first;

    while (p)
    {
        aux = p;
        p = p->next;
        free(aux);
    }
    free(*q);
    *q = NULL;
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

    for (int i = startX; i < startX + size; i++)
        for (int j = startY; j < startY + size; j++)
        {
            int red = (int)(avgColor.red - imageMatrix[i][j].red);
            int green = (int)(avgColor.green - imageMatrix[i][j].green);
            int blue = (int)(avgColor.blue - imageMatrix[i][j].blue);

            mean += red * red + green * green + blue * blue;
        }

    mean = mean / (unsigned long long)(3 * size * size);

    return mean;
}

// read the arg. when executing
void ReadExecArg(int argc, char *argv[])
{
    TTree arb = NULL;
    RGB **imageMatrix = NULL;
    unsigned int width = zero, height = zero, nodeMaxSize = zero;

    if (strcmp(argv[1], "-c1") == 0)
    {
        int similarity = atoi(argv[2]);

        ReadPPMfile(argv[3], &imageMatrix, &width, &height);
        CompressImage(&imageMatrix, &arb, width, zero, zero, similarity, &nodeMaxSize);
        WriteInfoTree(argv[4], arb, nodeMaxSize);
    }
    if (strcmp(argv[1], "-c2") == 0)
    {
        int similarity = atoi(argv[2]);

        ReadPPMfile(argv[3], &imageMatrix, &width, &height);
        CompressImage(&imageMatrix, &arb, width, zero, zero, similarity, &nodeMaxSize);
        WriteCompressedFile(argv[4], arb, width);
    }
    if (strcmp(argv[1], "-d") == 0)
    {
        DecompressImage(argv[2], argv[3]);
    }

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
        int length = size;
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

// counting the nodes which contains color
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

// creating a .out file with some info about the quadtree
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

// storing the values of the quadtree in a binary format
void WriteCompressedFile(char *fileName, TTree arb, unsigned int size)
{
    FILE *file = fopen(fileName, "wb");
    if (!file)
    {
        printf("Error at opening the file\n");
        return;
    }

    fwrite(&size, sizeof(unsigned int), 1, file);

    // store in a transversal order the values of the quadtree
    TQueue *q = InitQueue();
    TTree tree = arb;
    unsigned char value = 0;

    if (!arb)
        return;

    while (tree != NULL)
    {
        if (tree->type == ColorNode)
        {
            value = 1;
            fwrite(&value, 1, sizeof(unsigned char), file);
            fwrite(&((RGB *)tree->info)->red, 1, sizeof(unsigned char), file);
            fwrite(&((RGB *)tree->info)->green, 1, sizeof(unsigned char), file);
            fwrite(&((RGB *)tree->info)->blue, 1, sizeof(unsigned char), file);
        }
        else
        {
            value = 0;
            fwrite(&value, 1, sizeof(unsigned char), file);
            AddQueue(q, &(tree->topLeft));
            AddQueue(q, &(tree->topRight));
            AddQueue(q, &(tree->botRight));
            AddQueue(q, &(tree->botLeft));
        }

        tree = ExtractQueue(q);
    }

    DestroyQueue(&q);

    fflush(file);
    fclose(file);
}

TTree RestoreQuadTree(FILE *file)
{
    TQueue *q = InitQueue();
    TTree arb = NULL;
    unsigned char value = 0;

    fread(&value, 1, sizeof(unsigned char), file);
    if (value == 1)
    {
        unsigned char red, green, blue;
        fread(&red, 1, sizeof(unsigned char), file);
        fread(&green, 1, sizeof(unsigned char), file);
        fread(&blue, 1, sizeof(unsigned char), file);
        arb = InitCNode(red, green, blue);

        DestroyQueue(&q);
        return arb;
    }
    else
    {
        arb = InitNode();
        AddQueue(q, &arb);
    }

    while (q->first != NULL)
    {
        TTree tree = ExtractQueue(q);

        fread(&value, 1, sizeof(unsigned char), file);
        if (value == 1)
        {
            unsigned char red, green, blue;
            fread(&red, 1, sizeof(unsigned char), file);
            fread(&green, 1, sizeof(unsigned char), file);
            fread(&blue, 1, sizeof(unsigned char), file);
            tree->topLeft = InitCNode(red, green, blue);
        }
        if (value == 0)
        {
            tree->topLeft = InitNode();
            AddQueue(q, &(tree->topLeft));
        }

        fread(&value, 1, sizeof(unsigned char), file);
        if (value == 1)
        {
            unsigned char red, green, blue;
            fread(&red, 1, sizeof(unsigned char), file);
            fread(&green, 1, sizeof(unsigned char), file);
            fread(&blue, 1, sizeof(unsigned char), file);
            tree->topRight = InitCNode(red, green, blue);
        }
        if (value == 0)
        {
            tree->topRight = InitNode();
            AddQueue(q, &(tree->topRight));
        }

        fread(&value, 1, sizeof(unsigned char), file);
        if (value == 1)
        {
            unsigned char red, green, blue;
            fread(&red, 1, sizeof(unsigned char), file);
            fread(&green, 1, sizeof(unsigned char), file);
            fread(&blue, 1, sizeof(unsigned char), file);
            tree->botRight = InitCNode(red, green, blue);
        }
        if (value == 0)
        {
            tree->botRight = InitNode();
            AddQueue(q, &(tree->botRight));
        }

        fread(&value, 1, sizeof(unsigned char), file);
        if (value == 1)
        {
            unsigned char red, green, blue;
            fread(&red, 1, sizeof(unsigned char), file);
            fread(&green, 1, sizeof(unsigned char), file);
            fread(&blue, 1, sizeof(unsigned char), file);
            tree->botLeft = InitCNode(red, green, blue);
        }
        if (value == 0)
        {
            tree->botLeft = InitNode();
            AddQueue(q, &(tree->botLeft));
        }
    }

    DestroyQueue(&q);

    return arb;
}

void RestoreImageMatrix(FILE *file, TTree arb, RGB ***imageMatrix, unsigned int size, unsigned int startX, unsigned int startY)
{
    if (!arb)
        return;

    if (arb->type == ColorNode)
    {
        for (int i = startX; i < startX + size; i++)
            for (int j = startY; j < startY + size; j++)
            {
                (*imageMatrix)[i][j].red = ((RGB *)arb->info)->red;
                (*imageMatrix)[i][j].green = ((RGB *)arb->info)->green;
                (*imageMatrix)[i][j].blue = ((RGB *)arb->info)->blue;
            }
    }
    else
    {
        RestoreImageMatrix(file, arb->topLeft, imageMatrix, size / 2, startX, startY);
        RestoreImageMatrix(file, arb->topRight, imageMatrix, size / 2, startX, startY + size / 2);
        RestoreImageMatrix(file, arb->botLeft, imageMatrix, size / 2, startX + size / 2, startY);
        RestoreImageMatrix(file, arb->botRight, imageMatrix, size / 2, startX + size / 2, startY + size / 2);
    }
}

// decompressing the image
void DecompressImage(char *inFile, char *outFile)
{
    unsigned int size = 0;
    TTree arb = NULL;
    FILE *in = fopen(inFile, "rb");
    if (!in)
    {
        printf("Error at opening the file\n");
        return;
    }
    FILE *out = fopen(outFile, "wb");
    if (!out)
    {
        printf("Error at opening the file\n");
        return;
    }

    fread(&size, sizeof(unsigned int), 1, in);
    RGB **imageMatrix = InitImageMatrix(size, size);

    arb = RestoreQuadTree(in);
    RestoreImageMatrix(in, arb, &imageMatrix, size, 0, 0);

    fprintf(out, "P6\n%d %d\n255\n", size, size);

    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
        {
            fwrite(&imageMatrix[i][j].red, sizeof(unsigned char), 1, out);
            fwrite(&imageMatrix[i][j].green, sizeof(unsigned char), 1, out);
            fwrite(&imageMatrix[i][j].blue, sizeof(unsigned char), 1, out);
        }

    DestroyImageMatrix(&imageMatrix, size);
    DestroyTree(&arb);

    fclose(in);
    fclose(out);
}