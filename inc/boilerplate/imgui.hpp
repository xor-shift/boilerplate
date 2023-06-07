#pragma once

#include <boilerplate/glfw.hpp>

#include <stuff/expected.hpp>

#include <GLFW/glfw3.h>
#include <bgfx/bgfx.h>
#include <imgui.h>

#include <chrono>
#include <memory>
#include <mutex>
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

}  // namespace bpt::imgui
