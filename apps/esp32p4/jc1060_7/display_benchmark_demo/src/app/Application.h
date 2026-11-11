#pragma once

#include "BenchmarkDemo.h"
#include "Services.h"

/// Portable application; the board is started by the target entry point.
class Application final
{
public:
    Application(const Services& services, const Settings& settings) : demo_(services, settings) {}

    Status initialize() { return demo_.initialize(); }
    void   update() { demo_.update(); }

private:
    benchmark_demo::BenchmarkDemo demo_;
};
