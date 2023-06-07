#include <boilerplate/platform/platform.hpp>

namespace bpt::detail {

struct platform_linux final : platform {
    constexpr ~platform_linux() = default;

    auto native_display_handle() -> void*;

    auto native_window_handle(GLFWwindow* window) -> void*;
};

using native_platform = platform_linux;

}// namespace bpt::detail
