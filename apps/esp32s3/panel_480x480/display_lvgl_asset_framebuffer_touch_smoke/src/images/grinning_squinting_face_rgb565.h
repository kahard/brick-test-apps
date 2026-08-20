#pragma once

#include <stdint.h>

typedef enum {
    BRICK_IMAGE_FORMAT_RGB565_LE = 1,
    BRICK_IMAGE_FORMAT_RGBA8888 = 2,
} brick_image_pixel_format_t;

typedef struct {
    uint16_t width;
    uint16_t height;
    uint8_t format;
    uint8_t bytes_per_pixel;
    uint32_t data_size;
    const uint8_t* data;
} brick_image_asset_t;

extern const brick_image_asset_t brick_grinning_face;
