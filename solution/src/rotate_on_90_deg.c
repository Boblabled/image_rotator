#include "rotate_on_90_deg.h"
#include "image_format.h"
#include <stdlib.h>

struct pixel* get_pixel(struct image const* img, uint64_t x, uint64_t y){
    return &(img->data[y * img->width + x]);
}

/* создаёт копию изображения, которая повёрнута на 90 градусов */
struct image rotate_on_90_deg(struct image const* source ) {
    struct image output = {0};
    output.width = source->height;
    output.height = source->width;
    output.data = malloc(sizeof(struct pixel) * source->width * source->height);
    for (uint64_t i = 0; i < source->width; i++) {
        for (uint64_t j = 0; j < source->height; j++) {
            uint64_t x = i;
            uint64_t y = (source->height - 1 - j);
            output.data[i * output.width + j] = *get_pixel(source, x, y);
        }
    }
    return output;
}
