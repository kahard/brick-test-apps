#pragma once
#include "SpiBenchmarkTest.h"
namespace esp12f_st7789_spi_benchmark_smoke
{
class Application final
{
public:
    bool initialize();
    void update();

private:
    Board            board_;
    SpiBenchmarkTest test_{ board_.display(), board_.time(), board_.logger() };
};
}  // namespace esp12f_st7789_spi_benchmark_smoke
