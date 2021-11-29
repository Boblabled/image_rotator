#include "image_format.h"
#include "BMP_format.h"
#include "rotate_on_90_deg.h"
#include <stdio.h>

int main(int argc, char **argv) {
    (void) argc;
    (void) argv; // supress 'unused parameters' warning

    if (argc != 3) {
        printf("[ERROR] - incorrect input format\n");
        printf("[INFO] - correct input format: ./image-transformer <source-image> <transformed-image>\n");
        return 1;
    }

    struct image image = {0};

    const char * input_path = argv[1];
    const char * output_path = argv[2];

    FILE *in = {0};

    if (print_file_read_image_status(read_file(&in, input_path))) return 1;
    if (print_bmp_read_status(from_bmp(in, &image))) return 2;
    if (print_file_close_image_status(close_file(&in))) return 3;

    struct image image_rotated = rotate_on_90_deg(&image);
    free_image_data(&image);

    FILE *out = {0};

    if (print_file_write_image_status(write_file(&out, output_path))) return 4;
    if (print_bmp_write_status(to_bmp(out, &image_rotated))) return 5;
    if (print_file_close_image_status(close_file(&out))) return 6;

    free_image_data(&image_rotated);

    return 0;
}
