#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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

void **normalizeMatrice(int **matrice, int width, int height){
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

int ***extractMatriceIntoFramesX(int **matrice, int width, int height){
    int ***frameArray = (int ***) malloc((height - 2) * (width - 2) * sizeof(int **));

    for (int i = 0; i < height - 2; ++i) {
        for (int j = 0; j < width - 2; ++j) {
            frameArray[i * (width - 2) + j] = (int **) malloc(3 * sizeof(int *));
            for (int k = 0; k < 3; ++k) {
                frameArray[i * (width - 2) + j][k] = (int *) malloc(3 * sizeof(int));
                for (int l = 0; l < 3; ++l) {
                    frameArray[i * (width - 2) + j][k][l] = 0;
                }
            }
        }
    }
    for (int i = 0; i < height - 2; ++i) {
        for (int j = 0; j < width - 2; ++j) {
            frameArray[i * (width - 2) + j][0][0] = matrice[i][j];
            frameArray[i * (width - 2) + j][0][1] = matrice[i][j+1];
            frameArray[i * (width - 2) + j][0][2] = matrice[i][j+2];
            frameArray[i * (width - 2) + j][1][0] = matrice[i+1][j];
            frameArray[i * (width - 2) + j][1][1] = matrice[i+1][j+1];
            frameArray[i * (width - 2) + j][1][2] = matrice[i+1][j+2];
            frameArray[i * (width - 2) + j][2][0] = matrice[i+2][j];
            frameArray[i * (width - 2) + j][2][1] = matrice[i+2][j+1];
            frameArray[i * (width - 2) + j][2][2] = matrice[i+2][j+2];
        }
    }
    return frameArray;
}

int ***extractMatriceIntoFramesY(int **matrice, int width, int height){
    int ***frameArray = (int ***) malloc((height - 2) * (width - 2) * sizeof(int **));

    for (int i = 0; i < height - 2; ++i) {
        for (int j = 0; j < width - 2; ++j) {
            frameArray[i+ j * (width - 2) ] = (int **) malloc(3 * sizeof(int *));
            for (int k = 0; k < 3; ++k) {
                frameArray[i+ j * (width - 2) ][k] = (int *) malloc(3 * sizeof(int));
                for (int l = 0; l < 3; ++l) {
                    frameArray[i * (width - 2) + j][k][l] = 0;
                }
            }
        }
    }
    for (int j = 0; j < width - 2; ++j) {
        for (int i = 0; i < height - 2; ++i) {
            frameArray[i + j * (height - 2)][0][0] = matrice[i][j];
            frameArray[i + j * (height - 2)][0][1] = matrice[i][j+1];
            frameArray[i + j * (height - 2)][0][2] = matrice[i][j+2];
            frameArray[i + j * (height - 2)][1][0] = matrice[i+1][j];
            frameArray[i + j * (height - 2)][1][1] = matrice[i+1][j+1];
            frameArray[i + j * (height - 2)][1][2] = matrice[i+1][j+2];
            frameArray[i + j * (height - 2)][2][0] = matrice[i+2][j];
            frameArray[i + j * (height - 2)][2][1] = matrice[i+2][j+1];
            frameArray[i + j * (height - 2)][2][2] = matrice[i+2][j+2];
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
    printf("%s\n", arr);
    fgets(arr, 100, fptr);
    int width, height;
    char * suffix;
    char * token = strtok(arr, " ");
    width = (int) strtol(token, &suffix, 10);

    token = strtok(NULL, " ");
    height = (int) strtol(token, &suffix, 10);

    printf("%d %d\n", width, height);
    fgets(arr, 10000, fptr);

    int **imageContent = (int **) malloc(height * sizeof(int *));

    for (int i = 0; i < height; ++i) {
        imageContent[i] = (int *) malloc(width * sizeof(int));
    }

    for (int i = 0; i < height; ++i) {
        int j = 0;
        char value[4] = "\0";
        int c = getc(fptr);
        bool enteredInt = false;
        while (j < width){
            if (c>=48 && c<=57){
                strncat(value, (const char *) &c, 1);
                enteredInt = true;
            } else if (enteredInt){
                imageContent[i][j] = (int) strtol(value, &suffix, 10);
                strcpy(value, "");
                enteredInt = false;
                j++;
            }
            c = getc(fptr);
        }
    }

    for (int i = 0; i <height; ++i) {
        for (int j = 0; j < width; ++j) {
            printf("%d ", imageContent[i][j]);
        }
        printf("\n");
    }

    int ***frameArray = extractMatriceIntoFramesX(imageContent, width, height);

    for (int i = 0; i < (width - 2) * (height -2); ++i) {
        for (int j = 0; j < 3; ++j) {
            printf("%d %d %d\n", frameArray[i][j][0], frameArray[i][j][1], frameArray[i][j][2]);
        }
        printf("\n\n");
    }

    printf("===============\n");

    printf("%d\n", filterByX(frameArray[0]));

    FILE *fout;
    if ((fout = fopen("imageOutX.pgm", "w")) == NULL){
        perror("Error happened while opening file!");
        exit(1);
    }
    fprintf(fout, "P2\n%d %d\n255\n", width - 2, height - 2);

    int **filteredMatrice = (int **) malloc((height - 2) * sizeof(int *));

    for (int i = 0; i < height - 2; ++i) {
        filteredMatrice[i] = (int *) malloc((width - 2) * sizeof(int));
        for (int j = 0; j < width - 2; ++j) {
            filteredMatrice[i][j] = filterByX(frameArray[i * (width - 2) + j]);
        }
    }

    //normalizeMatrice(filteredMatrice, width - 2, height - 2);

    for (int i = 0; i < (height -2); ++i) {
        for (int j = 0; j < (width - 2); ++j) {
            if (filteredMatrice[i][j] < 0)
                fprintf(fout, "   0");
            else if (filteredMatrice[i][j]<10 && filteredMatrice[i][j]>=0)
                fprintf(fout, "   %d", filteredMatrice[i][j]);
            else if (filteredMatrice[i][j]<100 && filteredMatrice[i][j]>=10)
                fprintf(fout, "  %d", filteredMatrice[i][j]);
            else if (filteredMatrice[i][j]>=100 && filteredMatrice[i][j] <= 255)
                fprintf(fout, " %d", filteredMatrice[i][j]);
            else
                fprintf(fout, " 255");
        }
        fprintf(fout, "\n");
    }

    fclose(fout);

    if ((fout = fopen("imageOutY.pgm", "w")) == NULL){
        perror("Error happened while opening file!");
        exit(1);
    }
    fprintf(fout, "P2\n%d %d\n255\n", width - 2, height - 2);

    filteredMatrice = (int **) malloc((height - 2) * sizeof(int *));

    for (int i = 0; i < height - 2; ++i) {
        filteredMatrice[i] = (int *) malloc((width - 2) * sizeof(int));
        for (int j = 0; j < width - 2; ++j) {
            filteredMatrice[i][j] = filterByY(frameArray[i * (width - 2) + j]);
        }
    }

    //normalizeMatrice(filteredMatrice, width - 2, height - 2);

    for (int i = 0; i < (height -2); ++i) {
        for (int j = 0; j < (width - 2); ++j) {
            if (filteredMatrice[i][j] < 0)
                fprintf(fout, "   0");
            else if (filteredMatrice[i][j]<10 && filteredMatrice[i][j]>=0)
                fprintf(fout, "   %d", filteredMatrice[i][j]);
            else if (filteredMatrice[i][j]<100 && filteredMatrice[i][j]>=10)
                fprintf(fout, "  %d", filteredMatrice[i][j]);
            else if (filteredMatrice[i][j]>=100 && filteredMatrice[i][j] <= 255)
                fprintf(fout, " %d", filteredMatrice[i][j]);
            else
                fprintf(fout, " 255");
        }
        fprintf(fout, "\n");
    }

    return 0;
}
