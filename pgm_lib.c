#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

typedef struct{
    int width;
    int height;
    int **content;
    bool is_binary;
} Image;

const int SOBEL_FILTER_X[3][3] = {
        {-1, 0, 1},
        {-2, 0, 2},
        {-1, 0, 1}
};

const int SOBEL_FILTER_Y[3][3] = {
        {-1, -2, -1},
        {0, 0, 0},
        {1, 2, 1}
};

void normalize_image(Image *image);
Image add_padding_to_image(Image image);

void read_binary_pgm(Image *image, FILE *fptr){
    int i, j;
    char buffer[100], *suffix;
    fgets(buffer, 100, fptr);
    if (buffer[0] == '#')
        fgets(buffer, 100, fptr);
    char *token = strtok(buffer, " ");
    image->width = (int) strtol(token, &suffix, 10);
    token = strtok(NULL, " ");
    image->height = (int) strtol(token, &suffix, 10);
    fgets(buffer, 100, fptr);

    unsigned char *plain_image = (unsigned char *) malloc(image->width *
                                                          image->height * sizeof(unsigned char));

    fread(plain_image, sizeof(unsigned char), image->width * image->height, fptr);
    fgets(buffer, 10000, fptr);

    image->content = (int **) malloc(image->height * sizeof(int *));

    for (i = 0; i < image->height; ++i) {
        image->content[i] = (int *) malloc(image->width * sizeof(int));
        for (j = 0; j < image->width; ++j) {
            image->content[i][j] = plain_image[i * image->width + j];
        }
    }
}

void read_ascii_pgm(Image *image, FILE *fptr){
    int i;
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

    for (i = 0; i < image->height; ++i) {
        image->content[i] = (int *) malloc(image->width * sizeof(int));
        int j = 0;
        char value[4] = "\0";
        int c = getc(fptr);
        bool started_int = false;
        while (j < image->width){
            if (c>=48 && c<=57){
                strncat(value, (const char *) &c, 1);
                started_int = true;
            } else if (started_int){
                image->content[i][j] = (int) strtol(value, &suffix, 10);
                strcpy(value, "");
                started_int = false;
                j++;
            }
            c = getc(fptr);
        }
    }
}

Image read_pgm_file(char *filename){
    FILE *fptr;
    Image image;
    char buffer[5];
    if ((fptr = fopen(filename, "rb")) == NULL){
        perror("Error happened while opening file!");
        exit(1);
    }

    fgets(buffer, 5, fptr);

    if (buffer[1] == '2')
        read_ascii_pgm(&image, fptr);
    else
        read_binary_pgm(&image, fptr);
    fclose(fptr);

    normalize_image(&image);
    image = add_padding_to_image(image);
    return image;
}

void write_ascii_image_to_file(Image image, char *filename){
    int i, j;
    FILE *file;
    if ((file = fopen(filename, "w")) == NULL){
        perror("Error happened while opening file!");
        exit(1);
    }
    fprintf(file, "P2\n%d %d\n255\n", image.width, image.height);

    for (i = 0; i < image.height; ++i) {
        for (j = 0; j < image.width; ++j) {
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

void write_binary_image_to_file(Image image, char *filename){
    int i, j;
    FILE *file;
    if ((file = fopen(filename, "wb")) == NULL){
        perror("Error happened while opening file!");
        exit(1);
    }

    fprintf(file, "P5\n%d %d\n255\n", image.width, image.height);
    for (i = 0; i < image.height; ++i) {
        for (j = 0; j < image.width; ++j) {
            if (image.content[i][j] < 0)
                fprintf(file, "%c", 0);
            else if (image.content[i][j] > 255)
                fprintf(file, "%c", 255);
            else
                fprintf(file, "%c", image.content[i][j]);
        }
    }
}

void write_image(Image image, char *filename){
    if (image.is_binary)
        write_binary_image_to_file(image, filename);
    else
        write_ascii_image_to_file(image, filename);
}

void normalize_image(Image *image){
    int i, j;
    int min = image->content[0][0], max = image->content[0][0];
    for (i = 0; i < image->height; ++i) {
        for (j = 0; j < image->width; ++j) {
            if (min > image->content[i][j]) min = image->content[i][j];
            if (max < image->content[i][j]) max = image->content[i][j];
        }
    }
    int difference = max - min;

    for (i = 0; i < image->height; ++i) {
        for (j = 0; j < image->width; ++j) {
            if (difference == 0)
                image->content[i][j] = 255;
            else{
                int result = 255 * (image->content[i][j] - min) / (difference);
                image->content[i][j] = result;
            }
        }
    }
}

int filter_frame(int **frame, const int filter[3][3]){
    int i, j;
    int sum = 0;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            sum += filter[i][j] * frame[i][j];
        }
    }
    return sum;
}


int ***extract_matrix_into_frames(Image image){
    int i, j, k, l;
    int frame_array_height = image.height - 2;
    int frame_array_width = image.width - 2;
    int ***frame_array = (int ***) malloc(frame_array_height * frame_array_width * sizeof(int **));

    for (i = 0; i < frame_array_height; ++i) {
        for (j = 0; j < frame_array_width; ++j) {
            frame_array[i * frame_array_width + j] = (int **) malloc(3 * sizeof(int *));
            for (k = 0; k < 3; ++k) {
                frame_array[i * frame_array_width + j][k] = (int *) malloc(3 * sizeof(int));
                for (l = 0; l < 3; ++l) {
                    frame_array[i * frame_array_width + j][k][l] = 0;
                }
            }
        }
    }
    for (i = 0; i < frame_array_height; ++i) {
        for (j = 0; j < frame_array_width; ++j) {
            for (k = 0; k < 3; ++k) {
                for (l = 0; l < 3; ++l) {
                    frame_array[i * frame_array_width + j][k][l] = image.content[i + k][j + l];
                }
            }
        }
    }
    return frame_array;
}




Image filter_image(Image image, const int filter[3][3]){
    Image filtered_image;
    int i, j, ***frame_array = extract_matrix_into_frames(image);
    filtered_image.width = image.width - 2;
    filtered_image.height = image.height - 2;
    filtered_image.content = (int **) malloc(filtered_image.height * sizeof(int *));
    for (i = 0; i < filtered_image.height; ++i) {
        filtered_image.content[i] = (int *) malloc(filtered_image.width * sizeof(int));
        for (j = 0; j < filtered_image.width; ++j) {
            filtered_image.content[i][j] = filter_frame(frame_array[i * filtered_image.width + j], filter);
        }
    }
    return filtered_image;
}

Image filter_image_x(Image image){
    return filter_image(image, SOBEL_FILTER_X);
}

Image filter_image_y(Image image){
    return filter_image(image, SOBEL_FILTER_Y);
}


Image add_padding_to_image(Image image){
    int i, j;
    Image padded_image;
    padded_image.width = image.width + 2;
    padded_image.height = image.height + 2;
    padded_image.content = (int **) malloc(padded_image.height * sizeof(int *));

    for (i = 0; i < padded_image.height; ++i) {
        padded_image.content[i] = (int *) malloc(padded_image.width * sizeof(int));
        for (j = 0; j < padded_image.width; ++j) {
            padded_image.content[i][j] = 0;
        }
    }

    for (i = 0; i < image.height; ++i) {
        for (j = 0; j < image.width; ++j) {
            padded_image.content[i + 1][j + 1] = image.content[i][j];
        }
    }

    for (i = 0; i < image.width; ++i) {
        padded_image.content[0][i] = image.content[0][i];
        padded_image.content[padded_image.height - 1][i] = image.content[image.height - 1][i];
    }

    for (i = 0; i < image.height; ++i) {
        padded_image.content[i][0] = image.content[i][0];
        padded_image.content[i][padded_image.width - 1] = image.content[i][image.width - 1];
    }

    padded_image.content[0][0] = 0;
    padded_image.content[0][padded_image.width - 1] = 0;
    padded_image.content[padded_image.height - 1][0] = 0;
    padded_image.content[padded_image.height - 1][padded_image.width - 1] = 0;

    return padded_image;
}

Image merge_images(Image *image_x, Image *image_y){
    int i, j;
    Image merged_image;
    merged_image.width = image_x->width;
    merged_image.height = image_x->height;

    merged_image.content = (int **) malloc(merged_image.height * sizeof(int *));

    for (i = 0; i < merged_image.height; ++i) {
        merged_image.content[i] = (int *) malloc(merged_image.width * sizeof(int));
        for (j = 0; j < merged_image.width; ++j) {
            merged_image.content[i][j] = (int) sqrt((double) (image_x->content[i][j] * image_x->content[i][j] +
                    image_y->content[i][j] * image_y->content[i][j]));
        }
    }
    return merged_image;
}
