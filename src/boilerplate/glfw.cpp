#include <boilerplate/glfw.hpp>

#include <GLFW/glfw3.h>

#include <mutex>

namespace bpt::glfw {

template<typename T, usize N>
struct basic_circular_buffer {
    static constexpr usize queue_sz = 64;

    std::array<event_variant, queue_sz> events{};
    usize write_ptr = 0;
    usize usage = 0;

    constexpr auto full() const -> bool { return usage == queue_sz; }
    constexpr auto empty() const -> bool { return usage == 0; }
    constexpr auto size() const -> bool { return usage; }
    constexpr auto capacity() const -> bool { return queue_sz; }

    constexpr auto push(event_variant elem) -> bool {
        if (full()) {
            return false;
        }

        events[write_ptr++ % queue_sz] = elem;
        ++usage;

        return true;
    }

    constexpr auto pop() -> std::optional<event_variant> {
        if (empty()) {
            return std::nullopt;
        }

        auto elem = std::move(events[(write_ptr - usage) % queue_sz]);
        --usage;

        return elem;
    }
};

struct event_queue {
    static auto get() -> event_queue* {
        static auto* ptr = new event_queue();
        return ptr;
    }

    template<typename EventType, typename... Args>
    auto emit_event(GLFWwindow* window, Args&&... args) -> bool {
        auto event = EventType::from_glfw(std::forward<Args>(args)...);

        auto lock = std::unique_lock{m_mutex};

        auto& queue = m_queues[window];
        return queue.push(std::move(event));
    }

    auto pop_event(GLFWwindow* window) -> std::optional<event_variant> {
        auto lock = std::unique_lock{m_mutex};
        auto& queue = m_queues[window];
        return queue.pop();
    }

private:
    event_queue() {
        //
    }

    std::mutex m_mutex{};
    std::unordered_map<GLFWwindow*, basic_circular_buffer<event_variant, 64>> m_queues;
};

// clang-format off
static void glfw_callback_key(GLFWwindow* window, int key, int scancode, int action, int mods) { event_queue::get()->emit_event<event::key_input>(window, key, scancode, action, mods); }
static void glfw_callback_char(GLFWwindow* window, unsigned int codepoint) {event_queue::get()->emit_event<event::char_input>(window, codepoint);}
static void glfw_callback_cursor_position(GLFWwindow* window, double xpos, double ypos) { event_queue::get()->emit_event<event::cursor_position>(window, xpos, ypos); }
static void glfw_callback_cursor_enter(GLFWwindow* window, int entered) { event_queue::get()->emit_event<event::cursor_enter>(window, entered); }
static void glfw_callback_mouse_button(GLFWwindow* window, int button, int action, int mods) { event_queue::get()->emit_event<event::mouse_button>(window, button, action, mods); }
static void glfw_callback_scroll(GLFWwindow* window, double xoffset, double yoffset) { event_queue::get()->emit_event<event::scroll>(window, xoffset, yoffset); }
static void glfw_callback_drop(GLFWwindow* window, int count, const char** paths) { event_queue::get()->emit_event<event::drop>(window, count, paths); }
static void glfw_callback_window_close(GLFWwindow* window) { event_queue::get()->emit_event<event::window_close>(window); }
static void glfw_callback_window_size(GLFWwindow* window, int width, int height) { event_queue::get()->emit_event<event::window_size>(window, width, height); }
static void glfw_callback_framebuffer_size(GLFWwindow* window, int width, int height) { event_queue::get()->emit_event<event::framebuffer_size>(window, width, height); }
static void glfw_callback_window_content_scale(GLFWwindow* window, float xscale, float yscale) { event_queue::get()->emit_event<event::window_content_scale>(window, xscale, yscale); }
static void glfw_callback_window_pos(GLFWwindow* window, int xpos, int ypos) { event_queue::get()->emit_event<event::window_position>(window, xpos, ypos); }
static void glfw_callback_window_iconify(GLFWwindow* window, int iconified) { event_queue::get()->emit_event<event::window_iconify>(window, iconified); }
static void glfw_callback_window_maximize(GLFWwindow* window, int maximized) { event_queue::get()->emit_event<event::window_maximize>(window, maximized); }
static void glfw_callback_window_focus(GLFWwindow* window, int focused) { event_queue::get()->emit_event<event::window_focus>(window, focused); }
static void glfw_callback_window_refresh(GLFWwindow* window) { event_queue::get()->emit_event<event::window_refresh>(window); }
// clang-format on

static void register_glfw_callbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, glfw_callback_key);
    glfwSetCharCallback(window, glfw_callback_char);
    glfwSetCursorPosCallback(window, glfw_callback_cursor_position);
    glfwSetCursorEnterCallback(window, glfw_callback_cursor_enter);
    glfwSetMouseButtonCallback(window, glfw_callback_mouse_button);
    glfwSetScrollCallback(window, glfw_callback_scroll);
    glfwSetDropCallback(window, glfw_callback_drop);
    glfwSetWindowCloseCallback(window, glfw_callback_window_close);
    glfwSetWindowSizeCallback(window, glfw_callback_window_size);
    glfwSetFramebufferSizeCallback(window, glfw_callback_framebuffer_size);
    glfwSetWindowContentScaleCallback(window, glfw_callback_window_content_scale);
    glfwSetWindowPosCallback(window, glfw_callback_window_pos);
    glfwSetWindowIconifyCallback(window, glfw_callback_window_iconify);
    glfwSetWindowMaximizeCallback(window, glfw_callback_window_maximize);
    glfwSetWindowFocusCallback(window, glfw_callback_window_focus);
    glfwSetWindowRefreshCallback(window, glfw_callback_window_refresh);
}

window::window() {}

window::~window() {}

window::window(stf::blas::vector<int, 2> width, const char* title) { create(width, title); }

void window::create(stf::blas::vector<int, 2> width, const char* title) {
    if (m_handle != nullptr) {
        destroy();
    }

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    m_handle = glfwCreateWindow(width[0], width[1], title, nullptr, nullptr);
    if (m_handle == nullptr) {
        throw std::runtime_error("could not create a window");
    }

    register_glfw_callbacks(m_handle);
}

void window::destroy() {
    if (m_handle == nullptr) {
        return;
    }

    glfwDestroyWindow(m_handle);
}

auto window::key_is_pressed(int key) const -> bool { return glfwGetKey(m_handle, key) != GLFW_RELEASE; }

auto window::open() const -> bool { return glfwWindowShouldClose(m_handle) == GLFW_FALSE; }

auto window::size() const -> stf::blas::vector<int, 2> {
    auto ret = stf::blas::vector<int, 2>{};
    glfwGetWindowSize(m_handle, &ret[0], &ret[1]);
    return ret;
}

void window::poll_events() { glfwPollEvents(); }

auto window::pop_event() -> std::optional<event_variant> { return event_queue::get()->pop_event(m_handle); }

void deinit() {
    delete event_queue::get();
}

}  // namespace bpt::glfw
