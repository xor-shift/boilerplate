#include <boilerplate/imgui.hpp>

#include <stuff/expected.hpp>

#include <boilerplate/program.hpp>

#include <bx/bx.h>

#if BX_PLATFORM_LINUX
#    include <boilerplate/platform/linux.hpp>
#elif BX_PLATFORM_EMSCRIPTEN
#    include <boilerplate/platform/emscripten.hpp>
#endif

#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>
#include <bgfx/platform.h>
#include <imgui.h>

#include <stdexcept>
#include <unordered_map>

namespace bpt {

namespace detail {

static native_platform g_native_platform{};

}  // namespace detail

void program::init_bgfx(glfw::window& window) {
    auto init = detail::g_native_platform.get_base_init(window.get_handle());

    const auto window_size = window.size();
    init.resolution.width = (uint32_t)window_size[0];
    init.resolution.height = (uint32_t)window_size[1];
    init.resolution.reset = BGFX_RESET_NONE;

#ifdef EMSCRIPTEN
    init.type = bgfx::RendererType::WebGPU;
#else
    init.type = bgfx::RendererType::Vulkan;
#endif

    if (!bgfx::init(init)) {
        throw std::runtime_error("could not initialise bgfx");
    }
}

int program::main(int argc, char** argv, char** env) {
    auto frame_fn = [](void* arg) -> bool {
        auto& self = *reinterpret_cast<program*>(arg);
        return self.frame();
    };

    detail::g_native_platform.main_loop(frame_fn, reinterpret_cast<void*>(this));

    return 0;
}

static bpt::program* g_program = nullptr;

/// TODO: find a better approach to this shit-show
/// do note that we can't place the destructor at the end of main as emscripten has us quit main immediately
[[gnu::constructor(999)]] static void initialise_stuff() {
    glfwInit();
    bgfx::renderFrame();  // make bgfx single threaded
}

[[gnu::destructor(999)]] static void deinitialise_stuff() {
    delete g_program;
    g_program = nullptr;
    bpt::imgui::deinit();
    bgfx::shutdown();
    bpt::glfw::deinit();
    glfwTerminate();
}

int run(program* program, int argc, char** argv, char** env) {
    g_program = program;
    return g_program->main(argc, argv, env);
}

}  // namespace bpt
