#include "Application.h"

Application::Application()
    : board_(Board::double_buffered_config()),
      test_(board_.display(), static_cast<brick::interfaces::display::IFrameBufferDisplay&>(board_.display()),
            board_.touch(), board_.logger(), board_.time())
{
}

bool Application::initialize()
{
    return test_.initialize();
}

void Application::update()
{
    test_.update();
    board_.time().delay_ms(1);
}
