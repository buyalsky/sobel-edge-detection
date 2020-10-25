#include "pgm_lib.h"


int main() {
    Image sourceImage, outImageX, outImageY;
    readPgmFile(&sourceImage, "lena2.pgm", "rb");

    normalizeImage(&sourceImage);
    sourceImage = addPaddingToImage(sourceImage);

    outImageX = filterImage(sourceImage, sobelFilterX);
    outImageY = filterImage(sourceImage, sobelFilterY);

    Image outImage = mergeImages(&outImageX, &outImageY);

    normalizeImage(&outImageX);
    writeImage(outImageX, "imageOutX.pgm");
    normalizeImage(&outImageY);
    writeImage(outImageY, "imageOutY.pgm");
    normalizeImage(&outImage);
    writeImage(outImage, "imageOut.pgm");

    return 0;
}

