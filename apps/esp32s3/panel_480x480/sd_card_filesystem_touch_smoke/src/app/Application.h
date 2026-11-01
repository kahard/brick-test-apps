#pragma once

#include "SdCardTest.h"
#include "Services.h"

/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    explicit Application(const Services& services) : test_(services) {}

    Status initialize() { return test_.initialize(); }
    void   update() { test_.update(); }

private:
    SdCardTest test_;
};
