#pragma once

#include <boilerplate/glfw.hpp>

#include <stuff/expected.hpp>

#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>
#include <fmt/format.h>
#include <imgui.h>

#include <chrono>
#include <numbers>
#include <memory>
#include <mutex>
#include <type_traits>
#include <unordered_map>

struct GLFWwindow;

namespace bpt::imgui {

namespace detail {

struct imgui_user_texture_data {
    bgfx::TextureHandle handle;

    bool flag_alpha_blend : 1;
    bool padding0 : 7;
    bool padding1 : 8;

    static auto from_im_texture_id(ImTextureID tex_id) -> imgui_user_texture_data { return std::bit_cast<imgui_user_texture_data>((u32)std::bit_cast<std::uintptr_t>(tex_id)); }

    auto to_im_texture_id() const -> ImTextureID { return std::bit_cast<ImTextureID>((std::uintptr_t)std::bit_cast<u32>(*this)); }

    constexpr auto get_render_states() const -> u64 {
        constexpr auto alpha_state = BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_ALPHA, BGFX_STATE_BLEND_INV_SRC_ALPHA);
        constexpr auto msaa_rgba = BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_MSAA;

        auto ret = msaa_rgba;
        if (flag_alpha_blend) {
            ret |= alpha_state;
        }

        return ret;
    }
};

static_assert(sizeof(u32) == sizeof(imgui_user_texture_data));
static_assert(sizeof(void*) >= sizeof(imgui_user_texture_data));

}  // namespace detail

struct context {
    context();
    ~context();

    context(context const&) = delete;

    context(context&& other)
        : m_glfw_handle(other.m_glfw_handle)
        , m_im_context(other.m_im_context)
        , m_fonts_texture(other.m_fonts_texture)
        , m_view_id(other.m_view_id) {
        other.invalidate();
    }

    void init(glfw::window const& window);

    void new_frame(glfw::window& window);
    void render(glfw::window& window);
    auto process_event(glfw::window& window, glfw::event_variant const& event) -> bool;

    [[nodiscard("you mustn't discard a guard")]] auto make_current_context_guarded() -> auto{
        auto* const cur_ctx = ImGui::GetCurrentContext();
        ImGui::SetCurrentContext(m_im_context);
        return stf::scope::scope_exit{[prev_ctx = cur_ctx] { ImGui::SetCurrentContext(prev_ctx); }};
    }

private:
    GLFWwindow* m_glfw_handle = nullptr;
    ImGuiContext* m_im_context = nullptr;

    bgfx::TextureHandle m_fonts_texture{bgfx::kInvalidHandle};
    bgfx::ViewId m_view_id = 255;

    std::chrono::time_point<std::chrono::system_clock> m_last_update_tp;

    void invalidate() {
        m_glfw_handle = nullptr;
        m_im_context = nullptr;
        m_fonts_texture.idx = bgfx::kInvalidHandle;
    }

    auto is_valid() const -> bool { return m_im_context != nullptr; }

    void load_fonts();

    static void update_io_modifiers(glfw::window& window, ImGuiIO& io);

    static void set_view_transform(bgfx::ViewId view_id, const ImDrawData* draw_data);
};

void image(bgfx::TextureHandle handle, float width, float height);

void deinit();

// utilities

template<typename T>
struct data_type;

#pragma push_macro("IMGUI_TYPE_FACTORY")

#define IMGUI_TYPE_FACTORY(_std_type, _imgui_type) \
    template<>                                     \
    struct data_type<_std_type> : std::integral_constant<ImGuiDataType, ImGuiDataType_##_imgui_type> {};

IMGUI_TYPE_FACTORY(i8, S8);
IMGUI_TYPE_FACTORY(i16, S16);
IMGUI_TYPE_FACTORY(i32, S32);
IMGUI_TYPE_FACTORY(i64, S64);
IMGUI_TYPE_FACTORY(u8, U8);
IMGUI_TYPE_FACTORY(u16, U16);
IMGUI_TYPE_FACTORY(u32, U32);
IMGUI_TYPE_FACTORY(u64, U64);
IMGUI_TYPE_FACTORY(float, Float)
IMGUI_TYPE_FACTORY(double, Double)

#pragma pop_macro("IMGUI_TYPE_FACTORY")

template<typename T>
static constexpr ImGuiDataType data_type_v = data_type<T>::value;

template<typename T>
static auto input_scalar(const char* label, T& v, std::optional<T> step = std::nullopt, std::optional<T> step_fast = std::nullopt) -> bool {
    T* step_arg = step ? &*step : nullptr;
    T* step_fast_arg = step_fast ? &*step_fast : nullptr;

    return ImGui::InputScalar(label, data_type_v<T>, &v, step_arg, step_fast_arg);
}

template<typename T, usize Extent = std::dynamic_extent>
static auto input_scalar_n(const char* label, std::span<T, Extent> elems, std::optional<T> step = std::nullopt, std::optional<T> step_fast = std::nullopt) -> bool {
    T* step_arg = step ? &*step : nullptr;
    T* step_fast_arg = step_fast ? &*step_fast : nullptr;

    return ImGui::InputScalarN(label, data_type_v<T>, elems.data(), elems.size(), step_arg, step_fast_arg);
}

template<typename T>
static auto slider(const char* label, T& value, std::optional<T> min = std::nullopt, std::optional<T> max = std::nullopt, const char* format = nullptr) {
    T* min_arg = min ? &*min : nullptr;
    T* max_arg = max ? &*max : nullptr;

    return ImGui::SliderScalar(label, data_type_v<T>, &value, min_arg, max_arg, format);
}

template<std::floating_point T>
static auto slider_angle(const char* label, T& radians, T degrees_min = -360, T degrees_max = 360) {
    const char* format = nullptr;

    if constexpr (std::is_same_v<T, float>) {
        format = "%.3f deg";
    } else if constexpr (std::is_same_v<T, double>) {
        format = "%.3lf deg";
    }


    T degrees = radians * 360 * T(0.5) * std::numbers::inv_pi_v<T>;
    bool value_changed = slider<T>(label, degrees, degrees_min, degrees_max, format);
    radians = degrees * 2 * std::numbers::pi_v<T> / 360;

    return value_changed;
}

static auto guarded_group() {
    ImGui::BeginGroup();
    return stf::scope::scope_exit{[] { ImGui::EndGroup(); }};
}

template<typename Fn>
static void group(Fn&& functor) {
    ImGui::BeginGroup();
    auto guard = stf::scope::scope_exit{[] { ImGui::EndGroup(); }};

    std::invoke(std::forward<Fn>(functor));
}

template<typename Fn>
inline void button(const char* label, const ImVec2& sz, Fn&& functor) {
    if (!ImGui::Button(label, sz)) {
        return;
    }

    std::invoke(std::forward<Fn>(functor));
}

template<typename Fn>
inline void menu_item(const char* label, Fn&& functor) {
    if (!ImGui::MenuItem(label)) {
        return;
    }

    std::invoke(std::forward<Fn>(functor));
}

template<typename Fn>
inline void menu(const char* label, Fn&& functor) {
    if (!ImGui::BeginMenu(label)) {
        return;
    }

    auto guard = stf::scope::scope_exit{[] { ImGui::EndMenu(); }};

    std::invoke(std::forward<Fn>(functor));
}

inline void text(const char* str) { ImGui::TextUnformatted(str); }

template<typename... Ts>
inline void text(fmt::format_string<Ts...> fmt, Ts&&... args) {
    ImGui::TextUnformatted(fmt::format(std::move(fmt), std::forward<Ts>(args)...).c_str());
}

}  // namespace bpt::imgui
