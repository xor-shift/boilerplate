#pragma once

#include <bgfx/bgfx.h>
#include <GLFW/glfw3.h>

#include <atomic>

namespace bpt::detail {

// Platform objects must have a lifetime equal to that of the program.
struct platform {
    virtual constexpr ~platform() = default;

    virtual constexpr auto native_display_handle() -> void* = 0;
    virtual constexpr auto native_window_handle(GLFWwindow* window) -> void* = 0;

    virtual void fps_hint(double fps = 0.) {
        m_fps = fps;
    }

    virtual void main_loop(bool(*frame_fn)(void*), void* arg);

    virtual auto get_base_init(GLFWwindow* window) -> bgfx::Init {
        auto init = bgfx::Init{};

        init.platformData.ndt = native_display_handle();
        init.platformData.nwh = native_window_handle(window);

        return init;
    }

private:
    std::atomic<double> m_fps = 60.;
};

}
