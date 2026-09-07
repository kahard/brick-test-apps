#include "AssetPlayback.h"

#include "AssetSources.h"
#include "FramebufferPresenter.h"
#include "generated_assets.h"

AssetPlayback::AssetPlayback(AssetSources& sources, FramebufferPresenter& presenter)
    : sources_(sources), presenter_(presenter)
{
}

bool AssetPlayback::initialize()
{
    const brick::interfaces::display::AssetDescriptor* asset = generated_assets::get(generated_assets::Id::joy_tears);
    return asset != nullptr && presenter_.initialize(*asset, sources_.flash());
}

bool AssetPlayback::advance_mode()
{
    mode_.advance();
    storage_ = mode_.storage();
    if (sources_.select(storage_) != nullptr)
        return true;
    fallback_to_flash();
    return false;
}

bool AssetPlayback::present_next()
{
    const bool                 second = (frame_ & 1U) != 0U;
    const generated_assets::Id id =
        backgrounds() ? (second ? generated_assets::Id::blue_background : generated_assets::Id::red_background) :
                        (second ? generated_assets::Id::sweat_smile : generated_assets::Id::joy_tears);
    const brick::interfaces::display::AssetDescriptor* asset  = generated_assets::get(id);
    brick::interfaces::display::IAssetSource*          source = sources_.select(storage_);

    bool presented = asset != nullptr && source != nullptr && presenter_.present(*asset, *source);
    if (!presented && storage_ == 2U)
    {
        fallback_to_flash();
        presented = asset != nullptr && presenter_.present(*asset, sources_.flash());
    }
    if (presented)
        ++frame_;
    return presented;
}

std::uint8_t AssetPlayback::storage() const
{
    return storage_;
}

bool AssetPlayback::backgrounds() const
{
    return mode_.backgrounds();
}

void AssetPlayback::fallback_to_flash()
{
    sources_.fallback_to_flash();
    storage_ = 0U;
    mode_.fallback_to_flash();
}
