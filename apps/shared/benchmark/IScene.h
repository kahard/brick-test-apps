#pragma once

#include <cstdint>

#include "lvgl.h"

namespace benchmark_demo
{

/// One benchmark scene. A scene fills whatever container it gets, so the
/// same scene runs full screen, in a quarter of the screen (the mixed scene)
/// and on boards with other screen sizes.
class IScene
{
public:
    virtual ~IScene() = default;

    virtual const char* name() const = 0;
    /// Creates the scene's objects inside parent, sized to its content area.
    virtual void create(lv_obj_t* parent) = 0;
    /// Moves the scene on by elapsed_ms; every call changes what is drawn.
    virtual void step(std::uint32_t elapsed_ms) = 0;
    /// Deletes the scene's objects; create() may be called again later.
    virtual void destroy() = 0;
};

}  // namespace benchmark_demo
