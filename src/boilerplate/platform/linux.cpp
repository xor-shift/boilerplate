#include <bx/bx.h>

#if BX_PLATFORM_LINUX

#include <boilerplate/platform/linux.hpp>

#define GLFW_EXPOSE_NATIVE_X11
#include <GLFW/glfw3native.h>

namespace bpt::detail {

auto platform_linux::native_display_handle() -> void* { return glfwGetX11Display(); }

auto platform_linux::native_window_handle(GLFWwindow* window) -> void* { return (void*) (uintptr_t) glfwGetX11Window(window); }

}// namespace bpt::detail

#endif
