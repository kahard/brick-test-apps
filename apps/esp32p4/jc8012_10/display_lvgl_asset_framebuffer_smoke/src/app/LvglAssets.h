#pragma once
#include "PixelMemory.h"
#include "brick/platform/esp32/PartitionAssetSource.h"
#include "lvgl.h"

class LvglAssets final
{
public:
    bool                  initialize();
    const lv_image_dsc_t* image(bool second) const { return &descriptors_[second ? 1 : 0]; }

private:
    brick::platform::esp32::PartitionAssetSource source_{ "assets" };
    PixelMemory                                  pixels_[2];
    lv_image_dsc_t                               descriptors_[2]{};
};
