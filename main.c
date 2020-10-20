#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct Image{
    int width;
    int height;
    int **content;
} Image;

const int xAxis[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
};

const int yAxis[3][3] = {
        {1, 2, 1},
        {0, 0, 0},
        {-1, -2, -1}
};

void writeImageToFile(Image image, char *filename){
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
            else if (image.content[i][j]>=100 && image.content[i][j] <= 255)
                fprintf(file, " %d", image.content[i][j]);
            else
                fprintf(file, " 255");
        }
        fprintf(file, "\n");
    }
    fclose(file);
}

void normalizeMatrix(int **matrice, int width, int height){
    int min = matrice[0][0], max = matrice[0][0];
    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (min > matrice[i][j]) min = matrice[i][j];
            if (max < matrice[i][j]) max = matrice[i][j];
        }
    }

    int difference = max - min;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            if (difference == 0)
                matrice[i][j] = 255;
            else{
                int result = 255*(matrice[i][j] - min)/(difference);
                printf("%d\n", result);
                matrice[i][j] = result;

            }
        }
    }
}

int filterByX(int **frame){
    return xAxis[0][0] * frame[0][0] +
           xAxis[0][1] * frame[0][1] +
           xAxis[0][2] * frame[0][2] +
           xAxis[1][0] * frame[1][0] +
           xAxis[1][1] * frame[1][1] +
           xAxis[1][2] * frame[1][2] +
           xAxis[2][0] * frame[2][0] +
           xAxis[2][1] * frame[2][1] +
           xAxis[2][2] * frame[2][2];
}

int filterByY(int **frame){
    return yAxis[0][0] * frame[0][0] +
           yAxis[0][1] * frame[0][1] +
           yAxis[0][2] * frame[0][2] +
           yAxis[1][0] * frame[1][0] +
           yAxis[1][1] * frame[1][1] +
           yAxis[1][2] * frame[1][2] +
           yAxis[2][0] * frame[2][0] +
           yAxis[2][1] * frame[2][1] +
           yAxis[2][2] * frame[2][2];
}

int ***extractMatrixIntoFramesX(Image image){
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
            frameArray[i * frameArrayWidth + j][0][0] = image.content[i][j];
            frameArray[i * frameArrayWidth + j][0][1] = image.content[i][j+1];
            frameArray[i * frameArrayWidth + j][0][2] = image.content[i][j+2];
            frameArray[i * frameArrayWidth + j][1][0] = image.content[i+1][j];
            frameArray[i * frameArrayWidth + j][1][1] = image.content[i+1][j+1];
            frameArray[i * frameArrayWidth + j][1][2] = image.content[i+1][j+2];
            frameArray[i * frameArrayWidth + j][2][0] = image.content[i+2][j];
            frameArray[i * frameArrayWidth + j][2][1] = image.content[i+2][j+1];
            frameArray[i * frameArrayWidth + j][2][2] = image.content[i+2][j+2];
        }
    }
    return frameArray;
}

int main() {
    char arr[100];
    FILE *fptr;
    if ((fptr = fopen("lena.ascii.pgm", "r")) == NULL){
        perror("Error happened while opening file!");
        exit(1);
    }

    fgets(arr, 100, fptr);
    fgets(arr, 100, fptr);
    if (arr[0] == '#')
        fgets(arr, 100, fptr);
    Image sourceImage, outImageX, outImageY;
    char * suffix;
    char * token = strtok(arr, " ");
    sourceImage.width = (int) strtol(token, &suffix, 10);

    token = strtok(NULL, " ");
    sourceImage.height = (int) strtol(token, &suffix, 10);

    fgets(arr, 10000, fptr);

    sourceImage.content = (int **) malloc(sourceImage.height * sizeof(int *));

    for (int i = 0; i < sourceImage.height; ++i) {
        sourceImage.content[i] = (int *) malloc(sourceImage.width * sizeof(int));
    }

    for (int i = 0; i < sourceImage.height; ++i) {
        int j = 0;
        char value[4] = "\0";
        int c = getc(fptr);
        bool enteredInt = false;
        while (j < sourceImage.width){
            if (c>=48 && c<=57){
                strncat(value, (const char *) &c, 1);
                enteredInt = true;
            } else if (enteredInt){
                sourceImage.content[i][j] = (int) strtol(value, &suffix, 10);
                strcpy(value, "");
                enteredInt = false;
                j++;
            }
            c = getc(fptr);
        }
    }

    fclose(fptr);

    int ***frameArray = extractMatrixIntoFramesX(sourceImage);

    outImageX.width = sourceImage.width - 2;
    outImageX.height = sourceImage.height - 2;
    outImageX.content = (int **) malloc(outImageX.height * sizeof(int *));

    for (int i = 0; i < outImageX.height; ++i) {
        outImageX.content[i] = (int *) malloc(outImageX.width * sizeof(int));
        for (int j = 0; j < outImageX.width; ++j) {
            outImageX.content[i][j] = filterByX(frameArray[i * outImageX.width + j]);
        }
    }
    writeImageToFile(outImageX, "imageOutX.pgm");

    outImageY.width = sourceImage.width - 2;
    outImageY.height = sourceImage.height - 2;

    outImageY.content = (int **) malloc(outImageX.height * sizeof(int *));

    for (int i = 0; i < outImageX.height; ++i) {
        outImageY.content[i] = (int *) malloc(outImageY.width * sizeof(int));
        for (int j = 0; j < outImageY.width; ++j) {
            outImageY.content[i][j] = filterByY(frameArray[i * outImageY.width + j]);
        }
    }
    writeImageToFile(outImageY, "imageOutY.pgm");

    return 0;
}
