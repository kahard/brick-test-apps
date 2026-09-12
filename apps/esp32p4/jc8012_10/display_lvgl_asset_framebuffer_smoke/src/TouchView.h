#pragma once
#include "LvglAssets.h"
#include "brick/interfaces/logging/ILogger.h"

class TouchView final
{
public:
    TouchView(LvglAssets& assets, brick::interfaces::logging::ILogger& logger);
    void create();

private:
    static void                          clicked(lv_event_t* event);
    void                                 change_image();
    LvglAssets&                          assets_;
    brick::interfaces::logging::ILogger& logger_;
    lv_obj_t*                            image_  = nullptr;
    lv_obj_t*                            status_ = nullptr;
    std::uint32_t                        clicks_ = 0;
};
