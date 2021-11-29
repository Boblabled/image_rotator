#include"image_format.h"

enum file_read_image_status read_file (FILE** file, const char* path) {
    *file = fopen(path, "rb");
    return !*file ? FILE_READ_ERROR : FILE_READ_OK;
}

static char* const file_read_image_status_decoder[] = {
        [FILE_READ_OK] = "[INFO] - file successfully open to read\n",
        [FILE_READ_ERROR] = "[ERROR] - invalid file input\n"
};

enum file_read_image_status print_file_read_image_status(enum file_read_image_status status) {
    printf("%s", file_read_image_status_decoder[status]);
    return status;
}



enum file_write_image_status write_file (FILE** file, const char* path) {
    *file = fopen(path, "wb");
    return !*file ? FILE_WRITE_ERROR : FILE_WRITE_OK;
}

static char* const file_write_image_status_decoder[] = {
        [FILE_WRITE_OK] = "[INFO] - file successfully open to write\n",
        [FILE_WRITE_ERROR] = "[ERROR] - invalid file input\n"
};

enum file_write_image_status print_file_write_image_status(enum file_write_image_status status) {
    printf("%s", file_write_image_status_decoder[status]);
    return status;
}



enum file_close_image_status close_file (FILE** file) {
    if (!*file) {
        return FILE_CLOSE_ERROR;
    }
    fclose(*file);
    return FILE_CLOSE_OK;
}

static char* const file_close_image_status_decoder[] = {
        [FILE_CLOSE_OK] = "[INFO] - file successfully closed\n",
        [FILE_CLOSE_ERROR] = "[ERROR] - file already closed\n"
};

enum file_close_image_status print_file_close_image_status(enum file_close_image_status status) {
    printf("%s", file_close_image_status_decoder[status]);
    return status;
}
