#ifndef IMAGE_ROTATOR_IMAGE_FORMAT_H
#define IMAGE_ROTATOR_IMAGE_FORMAT_H
#include <stdio.h>
#include <stdint.h>

#pragma pack(push, 1)
struct pixel {
    uint8_t b, g, r;
};
#pragma pack(pop)

struct image {
    uint64_t width, height;
    struct pixel* data;
};

enum file_read_image_status{
    FILE_READ_OK = 0,
    FILE_READ_ERROR
};

enum file_write_image_status{
    FILE_WRITE_OK = 0,
    FILE_WRITE_ERROR
};

enum file_close_image_status{
    FILE_CLOSE_OK = 0,
    FILE_CLOSE_ERROR
};

enum file_read_image_status read_file (FILE** file, const char* path);
enum file_write_image_status write_file (FILE** file, const char* path);
enum file_close_image_status close_file (FILE** file);

void print_file_read_image_status(enum file_read_image_status status);
void print_file_write_image_status(enum file_write_image_status status);
void print_file_close_image_status(enum file_close_image_status status);

#endif //IMAGE_ROTATOR_IMAGE_FORMAT_H
