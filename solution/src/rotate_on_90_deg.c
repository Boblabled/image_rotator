//
// Created by Maxim Abramov on 28.11.2021.
//

#include <stdlib.h>
#include "rotate_on_90_deg.h"
#include "image_format.h"

/* создаёт копию изображения, которая повёрнута на 90 градусов */
struct image rotate_on_90_deg(const struct image source) {
    struct image output = {0};
    output.width = source.height;
    output.height = source.width;
    output.data = malloc(sizeof(struct pixel) * source.width * source.height);
    for (size_t i = 0; i < source.width; i++) {
        for (size_t j = 0; j < source.height; j++) {
            output.data[i * output.width + j] =  (source.data[(source.height - 1 - j) * source.width + i]);
        }
    }
    return output;
}