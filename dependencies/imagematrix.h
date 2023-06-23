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



void DestroyImageMatrix(RGB ***imageMatrix, unsigned int height)
{
    for (int i = 0; i < height; i++)
        free((*imageMatrix)[i]);
    free(*imageMatrix);
    *imageMatrix = NULL;
}
