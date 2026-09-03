#include "Application.h"

bool Application::initialize()
{
    if (!board_.begin())
        return false;
    return test_.initialize();
}

void Application::update()
{
    test_.update();
}
