//Эти модули знают про модуль, описывающий struct image, но ничего не знают про
//трансформации. Поэтому можно будет добавлять новые трансформации не переписывая
//код для входных форматов.

#include "image_format.h"
#include "BMP_format.h"
#include <stdio.h>
#include <stdlib.h>

#pragma pack(push, 1)
struct bmp_header {
    uint16_t bfType; // 0x4d42 | 0x4349 | 0x5450
    uint32_t bfileSize; // размер файла
    uint32_t bfReserved; // 0
    uint32_t bOffBits; // смещение до поля данных,
    uint32_t biSize; // размер структуры в байтах: 40(BITMAPINFOHEADER) или 108(BITMAPV4HEADER) или 124(BITMAPV5HEADER)
    uint32_t biWidth; // ширина в точках
    uint32_t biHeight; // высота в точках
    uint16_t biPlanes; // всегда 1
    uint16_t biBitCount; // 0 | 1 | 4 | 8 | 16 | 24 | 32
    uint32_t biCompression; // BI_RGB | BI_RLE8 | BI_RLE4 | BI_BITFIELDS | BI_JPEG | BI_PNG реально используется лишь BI_RGB
    uint32_t biSizeImage; // Количество байт в поле данных (обычно 0)
    uint32_t biXPelsPerMeter; // горизонтальное разрешение, точек на дюйм
    uint32_t biYPelsPerMeter; // вертикальное разрешение, точек на дюйм
    uint32_t biClrUsed; // Количество используемых цветов
    uint32_t biClrImportant; // Количество существенных цветов (можно считать 0)
};
#pragma pack(pop)

struct bmp_header bmp_header_set(const struct image* img) {
   struct bmp_header bmpHeader = {
           .bfType = 0x4d42,
           .bfileSize = img->height * img->width * sizeof(struct pixel) +
                        img->height * (img->width % 4) + sizeof(struct bmp_header),
           .bfReserved = 0,
           .bOffBits = sizeof(struct bmp_header),
           .biSize = 40,
           .biWidth = img->width,
           .biHeight = img->height,
           .biPlanes = 1,
           .biBitCount = 24,
           .biCompression = 0,
           .biSizeImage =  img->height * img->width * sizeof(struct pixel) + (img->width % 4)*img->height,
           .biXPelsPerMeter = 0,
           .biYPelsPerMeter = 0,
           .biClrUsed = 0,
           .biClrImportant = 0,
   };
    return bmpHeader;
}

void free_image_data(struct image* img){
    free(img->data);
    img->data = NULL;
    img->width = 0;
    img->height = 0;
}

enum bmp_read_status bmp_header_check(FILE* in,  const struct bmp_header bmp_header ){
    fseek(in, 0, SEEK_END);
    uint32_t filesize = ftell(in);
    fseek(in, sizeof(struct bmp_header), SEEK_SET);
    /*
    if ((size != sizeof(struct bmp_header)) || (!in)) {
        return READ_INVALID_INPUT;
    }
     */

    if (bmp_header.bfType != 0x4d42 && bmp_header.bfType != 0x4349 && bmp_header.bfType != 0x5450) {
        return READ_INVALID_SIGNATURE;
    }

    if (bmp_header.biBitCount != 24) {
        return READ_INVALID_BITS;
    }

    if (bmp_header.bfileSize != filesize || bmp_header.bfReserved != 0 ||
        bmp_header.biPlanes != 1 || bmp_header.biSize != 40) {
        return READ_INVALID_HEADER;
    }

    if (bmp_header.biWidth < 1 || bmp_header.biWidth > 10000 ||
        bmp_header.biHeight < 1 || bmp_header.biHeight > 10000) {
        return READ_INVALID_SIZE;
    }
    return READ_OK;
}

enum bmp_read_status from_bmp(FILE* in, struct image* img) {
    struct bmp_header bmp_header = {0};
    fread(&bmp_header,  sizeof(struct bmp_header), 1, in);

    enum bmp_read_status header_check_status = bmp_header_check(in, bmp_header);
    if (header_check_status != READ_OK) {
        return header_check_status;
    }

    img->height = bmp_header.biHeight;
    img->width = bmp_header.biWidth;
    img->data = (struct pixel*) malloc(img->width * img->height * sizeof(struct pixel));

    for (size_t i = 0; i < img->height; i++) {
        fread(&(img->data[i*img->width]), sizeof(struct pixel), img->width, in);
        fseek(in, (uint8_t)img->width  % 4, SEEK_CUR);
    }

    return READ_OK;
}

enum bmp_write_status to_bmp(FILE* out, struct image const* img) {
    if(out == NULL) return WRITE_INVALID_INPUT;
    struct bmp_header bmp_header = bmp_header_set(img);

    bmp_header.biHeight = img->height;
    bmp_header.biWidth = img->width;

    fwrite(&bmp_header, sizeof(struct bmp_header), 1, out);

    const uint8_t zero = 0;
    for(size_t i = 0; i < img->height; i++) {
        fwrite(&(img->data[i * img->width]), sizeof(struct pixel), img->width, out);
        fwrite(&zero, 1, img->width % 4, out);
    }

    return WRITE_OK;
}

char* const read_status_decoder[] = {
        [READ_OK] = "[INFO] - bmp file successfully read\n",
        //[READ_INVALID_INPUT] = "[ERROR] - invalid input size\n",
        [READ_INVALID_SIGNATURE] = "[ERROR] - wrong format\n",
        [READ_INVALID_BITS] = "[ERROR] - invalid count of bits\n",
        [READ_INVALID_HEADER] = "[ERROR] - invalid header arguments\n",
        [READ_INVALID_SIZE] = "[ERROR] - too big size of image\n"
};

void print_bmp_read_status(enum bmp_read_status status) {
    printf("%s", read_status_decoder[status]);
}

char* const write_status_decoder[] = {
        [WRITE_OK] = "[INFO] - bmp file successfully wrote\n",
        [WRITE_INVALID_INPUT] = "[ERROR] - invalid file input\n"
};

void print_bmp_write_status(enum bmp_write_status status) {
    printf("%s", write_status_decoder[status]);
}
