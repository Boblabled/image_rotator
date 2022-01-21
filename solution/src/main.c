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

    FILE *in = NULL;

    enum file_read_image_status file_read_image = read_file(&in, input_path);
    print_file_read_image_status(file_read_image);
    if (file_read_image) {
        return file_read_image;
    }

    enum bmp_read_status bmp_read = from_bmp(in, &image);
    print_bmp_read_status(bmp_read);
    if (bmp_read) {
        if (bmp_read == READ_INVALID_WIDTH || bmp_read == READ_INVALID_INCORRECT_FORMAT) {
            free_image_data(&image);
        }

        enum file_close_image_status file_close_image_in = close_file(&in);
        print_file_close_image_status(file_close_image_in);
        if (file_close_image_in) {
            return file_close_image_in;
        }

        return bmp_read;
    }

    enum file_close_image_status file_close_image_in = close_file(&in);
    print_file_close_image_status(file_close_image_in);
    if (file_close_image_in) {
        return file_close_image_in;
    }

    struct image image_rotated = rotate_on_90_deg(&image);
    free_image_data(&image);

    FILE *out = NULL;

    enum file_write_image_status file_write_image = write_file(&out, output_path);
    print_file_write_image_status(file_write_image);
    if (file_write_image) {
        return file_write_image;
    }

    enum bmp_write_status bmp_write = to_bmp(out, &image_rotated);
    print_bmp_write_status(bmp_write);
    if (bmp_write) {
        free_image_data(&image_rotated);

        enum file_close_image_status file_close_image_out = close_file(&out);
        print_file_close_image_status(file_close_image_out);
        if (file_close_image_out) {
            return file_close_image_out;
        }

        return bmp_write;
    }

    enum file_close_image_status file_close_image_out = close_file(&out);
    print_file_close_image_status(file_close_image_out);
    if (file_close_image_out) {
        return file_close_image_out;
    }

    free_image_data(&image_rotated);

    return 0;
}
