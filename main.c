#include <getopt.h>
#include <string.h>
#include <stdlib.h>

#include "pgm_lib.h"


int main(int argc, char *argv[]) {
    int opt;
    char outputPath[20], inputPath[20];
    Image sourceImage, outImageX, outImageY;
    static char usage[] = "usage: %s -x [input file] -o [output file]\n";

    while ((opt = getopt(argc, argv, "x:o:")) != -1){
        switch (opt) {
            case 'o':
                strcpy(outputPath, optarg);
                break;
            case 'x':
                strcpy(inputPath, optarg);
                break;
            default:
                fprintf(stderr, usage, argv[0]);
                exit(1);
        }
    }

    readPgmFile(&sourceImage, inputPath, "rb");

    normalizeImage(&sourceImage);
    sourceImage = addPaddingToImage(sourceImage);

    outImageX = filterImage(sourceImage, sobelFilterX);
    outImageY = filterImage(sourceImage, sobelFilterY);

    Image outImage = mergeImages(&outImageX, &outImageY);

    normalizeImage(&outImageX);
    normalizeImage(&outImageY);
    normalizeImage(&outImage);

    writeImage(outImage, outputPath);

    return 0;
}

