#ifndef IMAGE_ROTATOR_BMP_FORMAT_H
#define IMAGE_ROTATOR_BMP_FORMAT_H
#include <stdio.h>

void free_image_data(struct image* img);

enum bmp_read_status {
    READ_OK = 0,
    READ_INVALID_INPUT,
    READ_INVALID_SIGNATURE,
    READ_INVALID_BITS,
    READ_INVALID_HEADER,
    READ_INVALID_INCORRECT_FORMAT,
    READ_INVALID_WIDTH
};

enum bmp_write_status {
    WRITE_OK = 0,
    WRITE_INVALID_INPUT,
    WRITE_INVALID_HEADER,
    WRITE_ERROR,
    WRITE_INVALID_PADDING
};

enum bmp_read_status from_bmp(FILE* in, struct image* img);
void print_bmp_read_status(enum bmp_read_status status);

enum bmp_write_status to_bmp(FILE* out, struct image const* img);
void print_bmp_write_status(enum bmp_write_status status);

#endif //IMAGE_ROTATOR_BMP_FORMAT_H
