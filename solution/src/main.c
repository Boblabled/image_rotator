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

    enum file_read_image_status file_read_status = read_file(&in, input_path);
    print_file_read_image_status(file_read_status);
    if (file_read_status) {
        return file_read_status;
    }

    enum bmp_read_status bmp_read_status = from_bmp(in, &image);
    print_bmp_read_status(bmp_read_status);
    if (bmp_read_status) {
        return bmp_read_status;
    }
    enum file_close_image_status file_in_close_status = close_file(&in);
    print_file_close_image_status(file_in_close_status);
    if (file_in_close_status) {
        return file_in_close_status;
    }

    struct image image_rotated = rotate_on_90_deg(image);

    free_image_data(&image);

    FILE *out = {0};

    enum file_write_image_status file_write_status = write_file(&out, output_path);
    print_file_write_image_status(file_write_status);
    if (file_write_status) {
        return file_write_status;
    }

    enum bmp_write_status bmp_write_status = to_bmp(out, &image_rotated);
    print_bmp_write_status(bmp_write_status);
    if (bmp_write_status) {
        return bmp_write_status;
    }

    enum file_close_image_status file_out_close_status = close_file(&out);
    print_file_close_image_status(file_out_close_status);
    if (file_out_close_status) {
        return file_out_close_status;
    }

    free_image_data(&image_rotated);

    return 0;
}
