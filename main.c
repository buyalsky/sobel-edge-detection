#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
    char arr[100];
    FILE *fptr;
    if ((fptr = fopen("image.pgm", "r")) == NULL){
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

    int imageContent[width][height];
    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < height; ++j) {
            imageContent[i][j] = getc(fptr);
        }
    }

    for (int i = 0; i <width; ++i) {
        for (int j = 0; j < height; ++j) {
            printf("%d", imageContent[i][j]);
        }
        printf("\n");
    }


    return 0;
}
