#include <array>
#include <cstdint>
#include <cstdio>

#include "brick/core/storage/StorageWriteVerify.h"
#include "brick/platform/esp32/p4/Jc1060Board.h"

extern "C" void app_main()
{
    brick::platform::esp32::p4::Jc1060Board board;
    if (!board.begin()) {
        std::printf("BOARD INIT FAIL\n");
        return;
    }
    std::printf("BOARD READY\n");
#if BRICK_JC1060_ENABLE_SDMMC
    auto& file_system = board.sdmmc();
    std::printf("SDMMC WRITE/READ VERIFY\n");
    if (!file_system.mount())
    {
        std::printf("SDMMC MOUNT FAIL\n");
        return;
    }
    std::array<std::uint8_t, 256> pattern{};
    for (std::size_t i = 0; i < pattern.size(); ++i)
        pattern[i] = static_cast<std::uint8_t>(i ^ 0x5AU);
    constexpr const char* kTestPath = "/sdcard/BRICK.BIN";
    const bool ok = brick::core::storage::write_verify(file_system,
                                                        kTestPath,
                                                        pattern.data(), pattern.size());
    std::printf("%s\n", ok ? "WRITE READ OK" : "WRITE READ FAIL");
    if (!ok)
    {
        auto diagnostic = file_system.open(kTestPath, "r");
        std::array<std::uint8_t, 256> readback{};
        const std::size_t read_count = diagnostic == nullptr ? 0U : diagnostic->read(readback.data(), 1U, readback.size());
        std::printf("DIAG open=%d read=%u first=%02X expected=%02X\n",
                    diagnostic != nullptr, static_cast<unsigned>(read_count), readback[0], pattern[0]);
    }
#else
    std::printf("SDMMC DISABLED\n");
#endif
}
