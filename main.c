#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
        {1, 2, 1},
        {0, 0, 0},
        {-1, -2, -1}
};

void readPgmFile(Image *image, char *filename, char *readMode);
void readBinaryPgm(Image *image, FILE *fptr);
void readAsciiPgm(Image *image, FILE *fptr);
void writeImage(Image image, char *filename);
void writeImageToAsciiFile(Image image, char *filename);
void writeImageToBinaryFile(Image image, char *filename);
int filterByX(int **frame);
int filterByY(int **frame);
int ***extractMatrixIntoFrames(Image image);
void normalizeMatrix(Image *image);


int main() {
    Image sourceImage, outImageX, outImageY;
    sourceImage.content = NULL;
    // TODO Padding for images
    readPgmFile(&sourceImage, "buffalo.pgm", "rb");

    normalizeMatrix(&sourceImage);
    writeImage(sourceImage, "imageOutB.pgm");

    int ***frameArray = extractMatrixIntoFrames(sourceImage);

    outImageX.width = sourceImage.width - 2;
    outImageX.height = sourceImage.height - 2;
    outImageX.content = (int **) malloc(outImageX.height * sizeof(int *));

    for (int i = 0; i < outImageX.height; ++i) {
        outImageX.content[i] = (int *) malloc(outImageX.width * sizeof(int));
        for (int j = 0; j < outImageX.width; ++j) {
            outImageX.content[i][j] = filterByX(frameArray[i * outImageX.width + j]);
        }
    }
    normalizeMatrix(&outImageX);
    writeImage(outImageX, "imageOutX.pgm");

    outImageY.width = sourceImage.width - 2;
    outImageY.height = sourceImage.height - 2;
    outImageY.content = (int **) malloc(outImageX.height * sizeof(int *));

    for (int i = 0; i < outImageX.height; ++i) {
        outImageY.content[i] = (int *) malloc(outImageY.width * sizeof(int));
        for (int j = 0; j < outImageY.width; ++j) {
            outImageY.content[i][j] = filterByY(frameArray[i * outImageY.width + j]);
        }
    }
    normalizeMatrix(&outImageY);
    writeImage(outImageY, "imageOutY.pgm");

    return 0;
}

void readBinaryPgm(Image *image, FILE *fptr){
    char buffer[100], *suffix;
    fgets(buffer, 100, fptr);
    if (buffer[0] == '#')
        fgets(buffer, 100, fptr);
    char *token = strtok(buffer, " ");
    image->width = (int) strtol(token, &suffix, 10);
    token = strtok(NULL, " ");
    image->height = (int) strtol(token, &suffix, 10);
    fgets(buffer, 100, fptr);

    unsigned char *plainImage = (unsigned char *) malloc(image->width *
                                                         image->height * sizeof(unsigned char));

    fread(plainImage, sizeof(unsigned char), image->width * image->height, fptr);
    fgets(buffer, 10000, fptr);

    image->content = (int **) malloc(image->height * sizeof(int *));

    for (int i = 0; i < image->height; ++i) {
        image->content[i] = (int *) malloc(image->width * sizeof(int));
        for (int j = 0; j < image->width; ++j) {
            image->content[i][j] = plainImage[i * image->width + j];
        }
    }
}

void readAsciiPgm(Image *image, FILE *fptr){
    char buffer[100];
    fgets(buffer, 100, fptr);
    if (buffer[0] == '#')
        fgets(buffer, 100, fptr);
    char *suffix;
    char *token = strtok(buffer, " ");
    image->width = (int) strtol(token, &suffix, 10);

    token = strtok(NULL, " ");
    image->height = (int) strtol(token, &suffix, 10);

    image->content = (int **) malloc(image->height * sizeof(int *));

    for (int i = 0; i < image->height; ++i) {
        image->content[i] = (int *) malloc(image->width * sizeof(int));
        int j = 0;
        char value[4] = "\0";
        int c = getc(fptr);
        bool enteredInt = false;
        while (j < image->width){
            if (c>=48 && c<=57){
                strncat(value, (const char *) &c, 1);
                enteredInt = true;
            } else if (enteredInt){
                image->content[i][j] = (int) strtol(value, &suffix, 10);
                strcpy(value, "");
                enteredInt = false;
                j++;
            }
            c = getc(fptr);
        }
    }
}

void readPgmFile(Image *image, char *filename, char *readMode){
    FILE *fptr;
    char buffer[5];
    if ((fptr = fopen(filename, readMode)) == NULL){
        perror("Error happened while opening file!");
        exit(1);
    }

    fgets(buffer, 5, fptr);

    if (buffer[1] == '2')
        readAsciiPgm(image, fptr);
    else
        readBinaryPgm(image, fptr);
    fclose(fptr);
}

void writeImageToAsciiFile(Image image, char *filename){
    FILE *file;
    if ((file = fopen(filename, "w")) == NULL){
        perror("Error happened while opening file!");
        exit(1);
    }
    fprintf(file, "P2\n%d %d\n255\n", image.width, image.height);

    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            if (image.content[i][j] < 0)
                fprintf(file, "   0");
            else if (image.content[i][j]<10 && image.content[i][j]>=0)
                fprintf(file, "   %d", image.content[i][j]);
            else if (image.content[i][j]<100 && image.content[i][j]>=10)
                fprintf(file, "  %d", image.content[i][j]);
            else if (image.content[i][j]>=100 && image.content[i][j]<=255)
                fprintf(file, " %d", image.content[i][j]);
            else
                fprintf(file, " 255");
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void writeImageToBinaryFile(Image image, char *filename){
    FILE *file;
    if ((file = fopen(filename, "wb")) == NULL){
        perror("Error happened while opening file!");
        exit(1);
    }

    fprintf(file, "P5\n%d %d\n255\n", image.width, image.height);
    for (int i = 0; i < image.height; ++i) {
        for (int j = 0; j < image.width; ++j) {
            if (image.content[i][j] < 0)
                fprintf(file, "%c", 0);
            else if (image.content[i][j] > 255)
                fprintf(file, "%c", 255);
            else
                fprintf(file, "%c", image.content[i][j]);
        }
    }
}

void writeImage(Image image, char *filename){
    if (image.isBinary)
        writeImageToBinaryFile(image, filename);
    else
        writeImageToAsciiFile(image, filename);
}

void normalizeMatrix(Image *image){
    int min = image->content[0][0], max = image->content[0][0];
    for (int i = 0; i < image->height; ++i) {
        for (int j = 0; j < image->width; ++j) {
            if (min > image->content[i][j]) min = image->content[i][j];
            if (max < image->content[i][j]) max = image->content[i][j];
        }
    }
    int difference = max - min;

    for (int i = 0; i < image->height; ++i) {
        for (int j = 0; j < image->width; ++j) {
            if (difference == 0)
                image->content[i][j] = 255;
            else{
                int result = 255 * (image->content[i][j] - min) / (difference);
                image->content[i][j] = result;
            }
        }
    }
}

int filterByX(int **frame){
    int sum = 0;
    for (int i = 0; i <3; ++i) {
        for (int j = 0; j < 3; ++j) {
            sum += sobelFilterX[i][j] * frame[i][j];
        }
    }
    return sum;
}

int filterByY(int **frame){
    int sum = 0;
    for (int i = 0; i <3; ++i) {
        for (int j = 0; j < 3; ++j) {
            sum += sobelFilterY[i][j] * frame[i][j];
        }
    }
    return sum;
}

int ***extractMatrixIntoFrames(Image image){
    int frameArrayHeight = image.height - 2;
    int frameArrayWidth = image.width - 2;
    int ***frameArray = (int ***) malloc(frameArrayHeight * frameArrayWidth * sizeof(int **));

    for (int i = 0; i < frameArrayHeight; ++i) {
        for (int j = 0; j < frameArrayWidth; ++j) {
            frameArray[i * frameArrayWidth + j] = (int **) malloc(3 * sizeof(int *));
            for (int k = 0; k < 3; ++k) {
                frameArray[i * frameArrayWidth + j][k] = (int *) malloc(3 * sizeof(int));
                for (int l = 0; l < 3; ++l) {
                    frameArray[i * frameArrayWidth + j][k][l] = 0;
                }
            }
        }
    }
    for (int i = 0; i < frameArrayHeight; ++i) {
        for (int j = 0; j < frameArrayWidth; ++j) {
            for (int k = 0; k < 3; ++k) {
                for (int l = 0; l < 3; ++l) {
                    frameArray[i * frameArrayWidth + j][k][l] = image.content[i+k][j+l];
                }
            }
        }
    }
    return frameArray;
}
