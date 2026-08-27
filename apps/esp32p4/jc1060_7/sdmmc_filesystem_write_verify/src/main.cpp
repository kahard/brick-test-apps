#include <array>
#include <cstdint>
#include <cstdio>

#include "brick/core/storage/StorageWriteVerify.h"
#include "brick/platform/esp32/p4/SdmmcFileSystem.h"

extern "C" void app_main()
{
    brick::platform::esp32::SdmmcFileSystem file_system;
    std::printf("SDMMC WRITE/READ VERIFY\n");
    if (!file_system.mount())
    {
        std::printf("SDMMC MOUNT FAIL\n");
        return;
    }
    std::array<std::uint8_t, 256> pattern{};
    for (std::size_t i = 0; i < pattern.size(); ++i)
        pattern[i] = static_cast<std::uint8_t>(i ^ 0x5AU);
    const bool ok = brick::core::storage::write_verify(file_system,
                                                        "/sdcard/brick_write_verify.bin",
                                                        pattern.data(), pattern.size());
    std::printf("%s\n", ok ? "WRITE READ OK" : "WRITE READ FAIL");
}

