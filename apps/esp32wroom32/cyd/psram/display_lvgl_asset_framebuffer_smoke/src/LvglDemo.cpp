#include "LvglDemo.h"

#include "generated_assets.h"

LV_FONT_DECLARE(brick_roboto_20);

namespace cyd_lvgl_asset_smoke
{
namespace
{
    constexpr char kTag[] = "cyd_lvgl_assets";
}

LvglDemo::LvglDemo(Board& board) : board_(board)
{
}

bool LvglDemo::load_selected_asset()
{
    const generated_assets::Id id =
        selected_ == 0 ? generated_assets::Id::joy_tears : generated_assets::Id::sweat_smile;
    const brick::interfaces::display::AssetDescriptor* asset = generated_assets::get(id);
    return asset != nullptr && asset->size == LvglBuffers::image_bytes() && assets_.load(*asset, buffers_.image());
}

void LvglDemo::on_button_clicked(lv_event_t* event)
{
    LvglDemo* demo = static_cast<LvglDemo*>(lv_event_get_user_data(event));
    demo->selected_ ^= 1U;

    if (!demo->load_selected_asset())
    {
        demo->board_.logger().error(kTag, "Unable to load selected asset");
        return;
    }

    ++demo->counter_;
    lv_image_set_src(demo->image_, &demo->image_descriptor_);
    lv_label_set_text_fmt(demo->status_, "Zmiana: %u", static_cast<unsigned>(demo->counter_));
}

void LvglDemo::create_widgets()
{
    lv_obj_t* screen = lv_screen_active();
    lv_obj_set_style_bg_color(screen, lv_color_hex(0x202040), 0);

    lv_obj_t* title = lv_label_create(screen);
    lv_label_set_text(title, "CYD PSRAM LVGL ASSETS FLASH");
    lv_obj_set_style_text_font(title, &brick_roboto_20, 0);
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_obj_align(title, LV_ALIGN_TOP_MID, 0, 12);

    image_ = lv_image_create(screen);
    lv_image_set_src(image_, &image_descriptor_);
    lv_obj_align(image_, LV_ALIGN_TOP_MID, 0, 42);

    lv_obj_t* button = lv_button_create(screen);
    lv_obj_set_size(button, 220, 58);
    lv_obj_align(button, LV_ALIGN_BOTTOM_MID, 0, -24);
    lv_obj_add_event_cb(button, on_button_clicked, LV_EVENT_CLICKED, this);

    lv_obj_t* button_label = lv_label_create(button);
    lv_label_set_text(button_label, "PRZELACZ");
    lv_obj_set_style_text_font(button_label, &brick_roboto_20, 0);
    lv_obj_center(button_label);

    status_ = lv_label_create(screen);
    lv_label_set_text(status_, "Asset: joy_tears");
    lv_obj_set_style_text_font(status_, &brick_roboto_20, 0);
    lv_obj_set_style_text_color(status_, lv_color_white(), 0);
    lv_obj_align(status_, LV_ALIGN_BOTTOM_MID, 0, -88);
}

bool LvglDemo::initialize()
{
    board_.logger().info(kTag, "Initializing CYD PSRAM LVGL asset demo");
    if (!buffers_.initialize() || !assets_.initialize() || !load_selected_asset())
    {
        board_.logger().error(kTag, "Unable to allocate buffers or load assets");
        return false;
    }

    image_descriptor_.header.cf     = LV_COLOR_FORMAT_RGB565;
    image_descriptor_.header.w      = 100;
    image_descriptor_.header.h      = 100;
    image_descriptor_.header.stride = 200;
    image_descriptor_.data_size     = static_cast<std::uint32_t>(LvglBuffers::image_bytes());
    image_descriptor_.data          = buffers_.image();

    lv_init();
    if (display_adapter_.create(LV_DISPLAY_RENDER_MODE_FULL, buffers_.first(), buffers_.second(),
                                static_cast<std::uint32_t>(LvglBuffers::frame_bytes()))
            == nullptr
        || touch_adapter_.create() == nullptr)
    {
        board_.logger().error(kTag, "Unable to initialize LVGL adapters");
        return false;
    }

    create_widgets();
    board_.logger().info(kTag, "LVGL demo ready");
    return true;
}

void LvglDemo::update()
{
    constexpr std::uint32_t kUpdatePeriodMs = 10;
    board_.time().delay_ms(kUpdatePeriodMs);
    lv_tick_inc(kUpdatePeriodMs);
    lv_timer_handler();
}
}  // namespace cyd_lvgl_asset_smoke
