// Hardware entry point: starts the board with its speaker, hands the display,
// touch screen and audio output to the portable application and runs it.
#include "../app/Application.h"
#include "BoardFeatures.h"
#include "brick/boards/esp32/s3/Panel480Board.h"

namespace
{
using Board           = brick::boards::Panel480BoardTemplate<audio_smoke::BoardFeatures>;
constexpr char kTag[] = "audio_smoke";
}  // namespace

extern "C" void app_main()
{
    static Board board;
    if (const brick::Status status = board.begin(); !is_ok(status))
    {
        board.logger().error(kTag, "Board initialization failed: %s", to_string(status));
        return;
    }
    static Application application({ board.display(), &board.touch(), board.audio(), board.time(), board.logger() });
    if (const brick::Status status = application.initialize(); !is_ok(status))
    {
        board.logger().error(kTag, "Application initialization failed: %s", to_string(status));
        return;
    }
    while (true)
    {
        application.update();
        board.time().delay_ms(2);
    }
}
