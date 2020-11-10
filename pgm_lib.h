#ifndef IMAGE_PROCESSING_ASG1_PGM_H
#define IMAGE_PROCESSING_ASG1_PGM_H
#endif //IMAGE_PROCESSING_ASG1_PGM_H
#include <stdbool.h>
#include <stdio.h>

//TODO: previously defined struct
typedef struct{
    int width;
    int height;
    int **content;
    bool is_binary;
} Image;


Image read_pgm_file(char *filename);
void write_image(Image image, char *filename);
Image filter_image_x(Image image);
Image filter_image_y(Image image);
void normalize_image(Image *image);
Image merge_images(Image *image_x, Image *image_y);
