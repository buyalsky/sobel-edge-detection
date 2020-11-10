#include <getopt.h>
#include <string.h>
#include <stdlib.h>

#include "pgm_lib.h"

static char USAGE[] = "usage: %s -x [input file] -o [output file] --export-gradients (optional)\n";

static struct option LONG_OPTIONS[] = {
        {"input", required_argument, 0, 'x'},
        {"out", required_argument, 0, 'o'},
        {"export-gradients", no_argument, 0, 'g'},
        {0, 0, 0, 0}
};


void print_usage(char *program_name){
    fprintf(stderr, USAGE, program_name);
}


int main(int argc, char *argv[]) {
    bool export_gradient = false;
    int opt;
    char output_filename[20], input_filename[20];
    Image source_image, out_image_x, out_image_y;

    if (argc<5){
        print_usage(argv[0]);
        exit(EXIT_FAILURE);
    }

    int long_index = 0;
    while ((opt = getopt_long(argc, argv, "x:o:g",
                              LONG_OPTIONS, &long_index)) != -1) {
        switch (opt) {
            case 'o':
                strcpy(output_filename, optarg);
                break;
            case 'x':
                strcpy(input_filename, optarg);
                break;
            case 'g' :
                export_gradient = true;
                break;
            default:
                print_usage(argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    source_image = read_pgm_file(input_filename);

    out_image_x = filter_image_x(source_image);
    out_image_y = filter_image_y(source_image);

    Image out_image = merge_images(&out_image_x, &out_image_y);

    normalize_image(&out_image_x);
    normalize_image(&out_image_y);
    normalize_image(&out_image);

    if (export_gradient){
        write_image(out_image_x, "gradientX.pgm");
        write_image(out_image_y, "gradientY.pgm");
    }
    write_image(out_image, output_filename);

    return 0;
}

