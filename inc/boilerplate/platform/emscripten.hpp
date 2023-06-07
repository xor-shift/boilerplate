#include <boilerplate/platform/platform.hpp>

namespace bpt::detail {

struct platform_emscripten final : platform {
    constexpr ~platform_emscripten() final override = default;

    constexpr auto native_display_handle() -> void* final override { return nullptr; }

    auto native_window_handle(GLFWwindow*) -> void* final override { return const_cast<void*>(reinterpret_cast<const void*>("#canvas")); }

    void main_loop(bool (*frame_fn)(void*), void* arg) final override;

    void fps_hint(double fps = 0.)final override;

private:
    bool (*m_frame_fn)(void*) = nullptr;
    void* m_frame_fn_arg = nullptr;
};

using native_platform = platform_emscripten;

}// namespace bpt::detail
