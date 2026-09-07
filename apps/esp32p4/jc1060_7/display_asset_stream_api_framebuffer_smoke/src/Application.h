#pragma once
#include "AssetSources.h"
#include "AssetStreamTest.h"
#include "FramebufferPresenter.h"
#include "Types.h"
class Application final
{
public:
    bool initialize();
    void update();

private:
    Board                board_;
    AssetSources         sources_{ board_.sdmmc() };
    FramebufferPresenter presenter_{ board_.display() };
    AssetPlayback        playback_{ sources_, presenter_ };
    AssetStreamTest      test_{ playback_, board_.touch(), board_.time(), board_.logger() };
};
