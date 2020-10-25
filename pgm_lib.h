#ifndef IMAGE_PROCESSING_ASG1_PGM_H
#define IMAGE_PROCESSING_ASG1_PGM_H
#endif //IMAGE_PROCESSING_ASG1_PGM_H
#include <stdbool.h>
#include <stdio.h>

typedef struct{
    int width;
    int height;
    int **content;
    bool isBinary;
} Image;


const int sobelFilterX[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
};

const int sobelFilterY[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
};

void readPgmFile(Image *image, char *filename, char *readMode);
void writeImage(Image image, char *filename);
Image filterImage(Image image, const int filter[3][3]);
void normalizeImage(Image *image);
Image addPaddingToImage(Image image);
Image mergeImages(Image *imageX, Image *imageY);
