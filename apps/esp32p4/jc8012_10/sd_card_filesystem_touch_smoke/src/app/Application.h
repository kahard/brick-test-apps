#pragma once

#include "SdmmcTest.h"
#include "Services.h"

/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    explicit Application(const Services& services) : test_(services) {}

    bool initialize() { return test_.initialize(); }
    void update() { test_.update(); }

private:
    SdmmcTest test_;
};
