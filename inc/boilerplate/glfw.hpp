#pragma once

#include <stuff/blas.hpp>

#include <GLFW/glfw3.h>

#include <optional>
#include <variant>

namespace bpt::glfw {

namespace event {

struct key_input {
    static auto from_glfw(int key, int scancode, int action, int mods) -> key_input { return {key, scancode, action, mods}; }

    int key;
    int scancode;
    int action;
    int mods;
};

struct char_input {
    static auto from_glfw(unsigned int codepoint) -> char_input { return {codepoint}; }

    unsigned int codepoint;
};

struct cursor_position {
    static auto from_glfw(double xpos, double ypos) -> cursor_position { return {{xpos, ypos}}; }

    stf::blas::vector<double, 2> position;
};

struct cursor_enter {
    static auto from_glfw(int entered) -> cursor_enter { return {entered == GLFW_TRUE}; }

    bool entered;
};

struct mouse_button {
    static auto from_glfw(int button, int action, int mods) -> mouse_button { return {button, action, mods}; }

    int button;
    int action;
    int mods;
};

struct scroll {
    static auto from_glfw(double xoffset, double yoffset) -> scroll { return {{xoffset, yoffset}}; }

    stf::blas::vector<double, 2> offset;
};

struct drop {
    static auto from_glfw(int count, const char** paths) -> drop { return {}; }

    // TODO
};

struct window_close {
    static auto from_glfw() -> window_close { return {}; }
};

struct framebuffer_size {
    static auto from_glfw(int width, int height) -> framebuffer_size { return {{width, height}}; }

    stf::blas::vector<int, 2> size;
};

struct window_content_scale {
    static auto from_glfw(float xscale, float yscale) -> window_content_scale { return {{xscale, yscale}}; }

    stf::blas::vector<int, 2> scale;
};

struct window_position {
    static auto from_glfw(int x, int y) -> window_position { return {{x, y}}; }

    stf::blas::vector<int, 2> position;
};

struct window_size {
    static auto from_glfw(int w, int h) -> window_size { return {{w, h}}; }

    stf::blas::vector<int, 2> size;
};

struct window_iconify {
    static auto from_glfw(int iconified) -> window_iconify { return {iconified == GLFW_TRUE}; }

    bool iconified;
};

struct window_maximize {
    static auto from_glfw(int maximized) -> window_maximize { return {maximized == GLFW_TRUE}; }

    bool maximized;
};

struct window_focus {
    static auto from_glfw(int focused) -> window_focus { return {focused == GLFW_TRUE}; }

    bool focused;
};

struct window_refresh {
    static auto from_glfw() -> window_refresh { return {}; }
};

}  // namespace event

using event_variant = std::variant<
  event::key_input,
  event::char_input,
  event::cursor_position,
  event::cursor_enter,
  event::mouse_button,
  event::scroll,
  event::drop,
  event::window_close,
  event::framebuffer_size,
  event::window_content_scale,
  event::window_position,
  event::window_size,
  event::window_iconify,
  event::window_maximize,
  event::window_focus,
  event::window_refresh>;

struct window {
    window();
    ~window();

    explicit window(stf::blas::vector<int, 2> width, const char* title);

    void create(stf::blas::vector<int, 2> width, const char* title);
    void destroy();

    constexpr auto get_handle() const -> GLFWwindow* { return m_handle; }

    auto open() const -> bool;
    auto size() const -> stf::blas::vector<int, 2>;

    auto key_is_pressed(int key) const -> bool;

    void poll_events();
    auto pop_event() -> std::optional<event_variant>;

private:
    GLFWwindow* m_handle = nullptr;
};

void deinit();

}  // namespace bpt::glfw
