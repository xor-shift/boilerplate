#include <boilerplate/detail/shader_helper.hpp>
#include <boilerplate/imgui.hpp>

#include <stuff/expected.hpp>
#include <stuff/scope.hpp>

#include <bgfx/bgfx.h>
#include <bx/math.h>
#include <imgui.h>

#include <fmt/format.h>

#include <filesystem>
#include <fstream>

namespace bpt::imgui::detail {

constexpr auto glfw_to_imgui_key(int glfw_key) -> std::optional<ImGuiKey> {
    switch (glfw_key) {
        case GLFW_KEY_SPACE: return ImGuiKey_Space;
        case GLFW_KEY_APOSTROPHE: return ImGuiKey_Apostrophe;
        case GLFW_KEY_COMMA: return ImGuiKey_Comma;
        case GLFW_KEY_MINUS: return ImGuiKey_Minus;
        case GLFW_KEY_PERIOD: return ImGuiKey_Period;
        case GLFW_KEY_SLASH: return ImGuiKey_Slash;
        case GLFW_KEY_0: return ImGuiKey_0;
        case GLFW_KEY_1: return ImGuiKey_1;
        case GLFW_KEY_2: return ImGuiKey_2;
        case GLFW_KEY_3: return ImGuiKey_3;
        case GLFW_KEY_4: return ImGuiKey_4;
        case GLFW_KEY_5: return ImGuiKey_5;
        case GLFW_KEY_6: return ImGuiKey_6;
        case GLFW_KEY_7: return ImGuiKey_7;
        case GLFW_KEY_8: return ImGuiKey_8;
        case GLFW_KEY_9: return ImGuiKey_9;
        case GLFW_KEY_SEMICOLON: return ImGuiKey_Semicolon;
        case GLFW_KEY_EQUAL: return ImGuiKey_Equal;
        case GLFW_KEY_A: return ImGuiKey_A;
        case GLFW_KEY_B: return ImGuiKey_B;
        case GLFW_KEY_C: return ImGuiKey_C;
        case GLFW_KEY_D: return ImGuiKey_D;
        case GLFW_KEY_E: return ImGuiKey_E;
        case GLFW_KEY_F: return ImGuiKey_F;
        case GLFW_KEY_G: return ImGuiKey_G;
        case GLFW_KEY_H: return ImGuiKey_H;
        case GLFW_KEY_I: return ImGuiKey_I;
        case GLFW_KEY_J: return ImGuiKey_J;
        case GLFW_KEY_K: return ImGuiKey_K;
        case GLFW_KEY_L: return ImGuiKey_L;
        case GLFW_KEY_M: return ImGuiKey_M;
        case GLFW_KEY_N: return ImGuiKey_N;
        case GLFW_KEY_O: return ImGuiKey_O;
        case GLFW_KEY_P: return ImGuiKey_P;
        case GLFW_KEY_Q: return ImGuiKey_Q;
        case GLFW_KEY_R: return ImGuiKey_R;
        case GLFW_KEY_S: return ImGuiKey_S;
        case GLFW_KEY_T: return ImGuiKey_T;
        case GLFW_KEY_U: return ImGuiKey_U;
        case GLFW_KEY_V: return ImGuiKey_V;
        case GLFW_KEY_W: return ImGuiKey_W;
        case GLFW_KEY_X: return ImGuiKey_X;
        case GLFW_KEY_Y: return ImGuiKey_Y;
        case GLFW_KEY_Z: return ImGuiKey_Z;
        case GLFW_KEY_LEFT_BRACKET: return ImGuiKey_LeftBracket;
        case GLFW_KEY_BACKSLASH: return ImGuiKey_Backslash;
        case GLFW_KEY_RIGHT_BRACKET: return ImGuiKey_RightBracket;
        case GLFW_KEY_GRAVE_ACCENT: return ImGuiKey_GraveAccent;
        case GLFW_KEY_ESCAPE: return ImGuiKey_Escape;
        case GLFW_KEY_ENTER: return ImGuiKey_Enter;
        case GLFW_KEY_TAB: return ImGuiKey_Tab;
        case GLFW_KEY_BACKSPACE: return ImGuiKey_Backspace;
        case GLFW_KEY_INSERT: return ImGuiKey_Insert;
        case GLFW_KEY_DELETE: return ImGuiKey_Delete;
        case GLFW_KEY_RIGHT: return ImGuiKey_RightArrow;
        case GLFW_KEY_LEFT: return ImGuiKey_LeftArrow;
        case GLFW_KEY_DOWN: return ImGuiKey_DownArrow;
        case GLFW_KEY_UP: return ImGuiKey_UpArrow;
        case GLFW_KEY_PAGE_UP: return ImGuiKey_PageUp;
        case GLFW_KEY_PAGE_DOWN: return ImGuiKey_PageDown;
        case GLFW_KEY_HOME: return ImGuiKey_Home;
        case GLFW_KEY_END: return ImGuiKey_End;
        case GLFW_KEY_CAPS_LOCK: return ImGuiKey_CapsLock;
        case GLFW_KEY_SCROLL_LOCK: return ImGuiKey_ScrollLock;
        case GLFW_KEY_NUM_LOCK: return ImGuiKey_NumLock;
        case GLFW_KEY_PRINT_SCREEN: return ImGuiKey_PrintScreen;
        case GLFW_KEY_PAUSE: return ImGuiKey_Pause;
        case GLFW_KEY_F1: return ImGuiKey_F1;
        case GLFW_KEY_F2: return ImGuiKey_F2;
        case GLFW_KEY_F3: return ImGuiKey_F3;
        case GLFW_KEY_F4: return ImGuiKey_F4;
        case GLFW_KEY_F5: return ImGuiKey_F5;
        case GLFW_KEY_F6: return ImGuiKey_F6;
        case GLFW_KEY_F7: return ImGuiKey_F7;
        case GLFW_KEY_F8: return ImGuiKey_F8;
        case GLFW_KEY_F9: return ImGuiKey_F9;
        case GLFW_KEY_F10: return ImGuiKey_F10;
        case GLFW_KEY_F11: return ImGuiKey_F11;
        case GLFW_KEY_F12: return ImGuiKey_F12;
        case GLFW_KEY_KP_0: return ImGuiKey_Keypad0;
        case GLFW_KEY_KP_1: return ImGuiKey_Keypad1;
        case GLFW_KEY_KP_2: return ImGuiKey_Keypad2;
        case GLFW_KEY_KP_3: return ImGuiKey_Keypad3;
        case GLFW_KEY_KP_4: return ImGuiKey_Keypad4;
        case GLFW_KEY_KP_5: return ImGuiKey_Keypad5;
        case GLFW_KEY_KP_6: return ImGuiKey_Keypad6;
        case GLFW_KEY_KP_7: return ImGuiKey_Keypad7;
        case GLFW_KEY_KP_8: return ImGuiKey_Keypad8;
        case GLFW_KEY_KP_9: return ImGuiKey_Keypad9;
        case GLFW_KEY_KP_DECIMAL: return ImGuiKey_KeypadDecimal;
        case GLFW_KEY_KP_DIVIDE: return ImGuiKey_KeypadDivide;
        case GLFW_KEY_KP_MULTIPLY: return ImGuiKey_KeypadMultiply;
        case GLFW_KEY_KP_SUBTRACT: return ImGuiKey_KeypadSubtract;
        case GLFW_KEY_KP_ADD: return ImGuiKey_KeypadAdd;
        case GLFW_KEY_KP_ENTER: return ImGuiKey_KeypadEnter;
        case GLFW_KEY_KP_EQUAL: return ImGuiKey_KeypadEqual;
        case GLFW_KEY_LEFT_SHIFT: return ImGuiKey_LeftShift;
        case GLFW_KEY_LEFT_CONTROL: return ImGuiKey_LeftCtrl;
        case GLFW_KEY_LEFT_ALT: return ImGuiKey_LeftAlt;
        case GLFW_KEY_LEFT_SUPER: return ImGuiKey_LeftSuper;
        case GLFW_KEY_RIGHT_SHIFT: return ImGuiKey_LeftShift;
        case GLFW_KEY_RIGHT_CONTROL: return ImGuiKey_RightCtrl;
        case GLFW_KEY_RIGHT_ALT: return ImGuiKey_RightAlt;
        case GLFW_KEY_RIGHT_SUPER: return ImGuiKey_RightSuper;
        case GLFW_KEY_MENU: return ImGuiKey_Menu;
        default: return std::nullopt;
    }
}  // namespace bpt::imgui::detail

constexpr auto glfw_to_imgui_mouse_button(int glfw_button) -> std::optional<ImGuiMouseButton> {
    switch (glfw_button) {
        case GLFW_MOUSE_BUTTON_1: return ImGuiMouseButton_Left;
        case GLFW_MOUSE_BUTTON_2: return ImGuiMouseButton_Right;
        case GLFW_MOUSE_BUTTON_3: return ImGuiMouseButton_Middle;
        default: return std::nullopt;
    }
}

constexpr auto glfw_to_imgui_mods(int glfw_mods) -> ImGuiModFlags {
    auto ret = ImGuiModFlags();

    ret |= (int)((glfw_mods & GLFW_MOD_SHIFT) != 0) * ImGuiModFlags_Shift;
    ret |= (int)((glfw_mods & GLFW_MOD_CONTROL) != 0) * ImGuiModFlags_Ctrl;
    ret |= (int)((glfw_mods & GLFW_MOD_ALT) != 0) * ImGuiModFlags_Alt;
    ret |= (int)((glfw_mods & GLFW_MOD_SUPER) != 0) * ImGuiModFlags_Super;
    // ret |= (int)((glfw_mods & GLFW_MOD_NUM_LOCK) != 0) * ImGuiModFlags_;

    return ret;
}

struct context_manager {
    friend context;

    static auto get() -> context_manager* {
        static auto* ptr = new context_manager();
        return ptr;
    }

    ~context_manager() {
        bgfx::destroy(m_tex);
        bgfx::destroy(m_program_imgui);
    }

    bgfx::VertexLayout m_vertex_layout;
    bgfx::ProgramHandle m_program_imgui;
    bgfx::UniformHandle m_tex;

private:
    static auto read_file_to_bgfx(const char* filename) -> std::optional<const bgfx::Memory*> {
        auto ifs = std::ifstream(filename);
        if (!ifs) {
            return std::nullopt;
        }

        auto file_data = std::vector<u8>{};
        std::copy(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>(), std::back_inserter(file_data));
        return bgfx::copy(file_data.data(), file_data.size());
    }

    context_manager() {
        m_vertex_layout.begin()
          .add(bgfx::Attrib::Position, 2, bgfx::AttribType::Float)
          .add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
          .add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
          .end();

        const auto imgui_shader_vs = load_embedded_shader([] {
            static  // lmao
#include <boilerplate/detail/generated_shaders/imgui.vs.essl.h>
              static
#include <boilerplate/detail/generated_shaders/imgui.vs.glsl.h>
              static
#include <boilerplate/detail/generated_shaders/imgui.vs.metal.h>
              static
#include <boilerplate/detail/generated_shaders/imgui.vs.vulkan.h>
              ;  // for clang-format

            return embedded_shader_pack{
              .essl = imgui_vs_essl,
              .glsl = imgui_vs_glsl,
              .metal = imgui_vs_metal,
              .vulkan = imgui_vs_vulkan,
            };
        });

        const auto imgui_shader_fs = load_embedded_shader([] {
            static
#include <boilerplate/detail/generated_shaders/imgui.fs.essl.h>
              static
#include <boilerplate/detail/generated_shaders/imgui.fs.glsl.h>
              static
#include <boilerplate/detail/generated_shaders/imgui.fs.metal.h>
              static
#include <boilerplate/detail/generated_shaders/imgui.fs.vulkan.h>
              ;

            return embedded_shader_pack{
              .essl = imgui_fs_essl,
              .glsl = imgui_fs_glsl,
              .metal = imgui_fs_metal,
              .vulkan = imgui_fs_vulkan,
            };
        });

        // const auto imgui_shader_vs = load_shader("shaders", "imgui", bpt::shader_type::vertex);
        // const auto imgui_shader_fs = load_shader("shaders", "imgui", bpt::shader_type::fragment);

        m_program_imgui = bgfx::createProgram(UNWRAP(imgui_shader_vs), UNWRAP(imgui_shader_fs), true);
        m_tex = bgfx::createUniform("m_tex", bgfx::UniformType::Sampler);
    }
};

}  // namespace bpt::imgui::detail

namespace bpt::imgui {

void deinit() { delete detail::context_manager::get(); }

context::context() {}

context::~context() {
    if (!is_valid()) {
        return;
    }

    ImGui::SetCurrentContext(nullptr);  // just in case

    bgfx::destroy(m_fonts_texture);
    ImGui::DestroyContext(m_im_context);

    invalidate();
}

void context::init(glfw::window const& window) {
    std::ignore = detail::context_manager::get();

    m_glfw_handle = window.get_handle();
    m_im_context = ImGui::CreateContext();

    auto context_guard = make_current_context_guarded();

    auto& io = ImGui::GetIO();
    io.DisplaySize = ImVec2(window.size()[0], window.size()[1]);
    io.DeltaTime = 1.f / 60.f;
    io.BackendFlags |= ImGuiBackendFlags_RendererHasVtxOffset;

    load_fonts();

    m_last_update_tp = std::chrono::system_clock::now();
}

void context::new_frame(glfw::window& window) {
    auto context_guard = make_current_context_guarded();

    ImGuiIO& io = ImGui::GetIO();
    const auto window_size = window.size();
    io.DisplaySize = ImVec2(window_size[0], window_size[1]);

    const auto now = std::chrono::system_clock::now();
    const auto time_delta = now - m_last_update_tp;
    m_last_update_tp = now;
    io.DeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(time_delta).count() / 1e6;

    ImGui::NewFrame();
}

void context::render(glfw::window& window) {
    const auto context_guard = make_current_context_guarded();
    auto& manager = *detail::context_manager::get();

    ImGui::Render();
    auto* const draw_data = ImGui::GetDrawData();

    const auto display_size = stf::blas::vector<float, 2>(draw_data->DisplaySize.x, draw_data->DisplaySize.y);
    const auto framebuffer_scale = stf::blas::vector<float, 2>(draw_data->FramebufferScale.x, draw_data->FramebufferScale.y);
    const auto framebuffer_size = display_size * framebuffer_scale;

    if (framebuffer_size[0] < 1.f || framebuffer_size[1] < 1.f)
        return;

    bgfx::setViewName(m_view_id, "ImGui");
    bgfx::setViewMode(m_view_id, bgfx::ViewMode::Sequential);
    set_view_transform(m_view_id, draw_data);

    std::for_each_n(draw_data->CmdLists, draw_data->CmdListsCount, [&](const ImDrawList* drawList) {
        const auto num_vertices = (u32)drawList->VtxBuffer.size();
        const auto num_indices = (u32)drawList->IdxBuffer.size();

        auto tvb = bgfx::TransientVertexBuffer{};
        bgfx::allocTransientVertexBuffer(&tvb, num_vertices, manager.m_vertex_layout);

        auto tib = bgfx::TransientIndexBuffer{};
        bgfx::allocTransientIndexBuffer(&tib, num_indices, sizeof(ImDrawIdx) == 4);

        std::copy_n(drawList->VtxBuffer.begin(), num_vertices, reinterpret_cast<ImDrawVert*>(tvb.data));
        std::copy_n(drawList->IdxBuffer.begin(), num_indices, reinterpret_cast<ImDrawIdx*>(tib.data));

        auto* const encoder = bgfx::begin();

        const auto clip_position = draw_data->DisplayPos;
        const auto clip_scale = draw_data->FramebufferScale;

        for (auto const& cmd : drawList->CmdBuffer) {
            if (cmd.UserCallback) {
                cmd.UserCallback(drawList, &cmd);
                continue;
            }

            if (cmd.ElemCount == 0) {
                continue;
            }

            if (cmd.TextureId != nullptr) {
                std::ignore = cmd.TextureId;
            }

            const auto texture_data = detail::imgui_user_texture_data::from_im_texture_id(cmd.TextureId);

            const auto clip_rect = ImVec4(
              (cmd.ClipRect.x - clip_position.x) * clip_scale.x,  //
              (cmd.ClipRect.y - clip_position.y) * clip_scale.y,  //
              (cmd.ClipRect.z - clip_position.x) * clip_scale.x,  //
              (cmd.ClipRect.w - clip_position.y) * clip_scale.y   //
            );

            if (clip_rect.x < framebuffer_size[0] && clip_rect.y < framebuffer_size[1] && clip_rect.z >= 0.f && clip_rect.w >= 0.f) {
                const auto x = std::max(clip_rect.x, 0.0f);
                const auto y = std::max(clip_rect.y, 0.0f);
                encoder->setScissor((u16)x, (u16)y, (u16)(std::min(clip_rect.z, 65535.f) - x), (u16)(std::min(clip_rect.w, 65535.f) - y));

                encoder->setState(texture_data.get_render_states());
                encoder->setTexture(0, manager.m_tex, texture_data.handle);
                encoder->setVertexBuffer(0, &tvb, cmd.VtxOffset, num_vertices);
                encoder->setIndexBuffer(&tib, cmd.IdxOffset, cmd.ElemCount);
                // encoder->submit(m_view_id, texture_data.is_texture_command() ? manager.m_program_imgui_image : manager.m_program_imgui);
                encoder->submit(m_view_id, manager.m_program_imgui);
            }
        }

        bgfx::end(encoder);
    });
}

auto context::process_event(glfw::window& window, glfw::event_variant const& event) -> bool {
    auto context_guard = make_current_context_guarded();

    auto& io = ImGui::GetIO();

    stf::multi_visitor event_visitor{
      [&window, &io](glfw::event::key_input event) {
          if (event.action != GLFW_PRESS && event.action != GLFW_RELEASE) {
              return;
          }

          update_io_modifiers(window, io);

          auto im_key = ({
              auto res = detail::glfw_to_imgui_key(event.key);
              if (!res) {
                  return;
              }
              *res;
          });

          io.AddKeyEvent(im_key, event.action == GLFW_PRESS);
      },
      [&io](glfw::event::cursor_position event) { io.AddMousePosEvent(event.position[0], event.position[1]); },
      [&io](glfw::event::mouse_button event) {
          auto res = detail::glfw_to_imgui_mouse_button(event.button);
          if (!res) {
              return;
          }
          io.AddMouseButtonEvent(*res, event.action == GLFW_PRESS);
      },
      [&io](glfw::event::scroll event) { io.AddMouseWheelEvent(event.offset[0], event.offset[1]); },
      [&io](glfw::event::char_input event) { io.AddInputCharacter(event.codepoint); },
      //[&window, &io](glfw::event:: event) {},
      [](auto const&) {},
    };

    std::visit(event_visitor, event);

    return false;
}

void context::load_fonts() {
    ImGuiIO& io = ImGui::GetIO();

    /*auto fonts = std::vector<ImFont*>{};

    for (auto directory = std::filesystem::directory_iterator("fonts"); std::filesystem::directory_entry const& entry : directory) {
        fmt::print("font entry: {}\n", entry.path().c_str());

        ImFontConfig config;
        config.FontDataOwnedByAtlas = false;
        config.MergeMode = false;

        auto ifs = std::ifstream(entry.path().c_str());
        auto file_data = std::vector<u8>{};
        std::copy(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>(), std::back_inserter(file_data));

        fonts.emplace_back(io.Fonts->AddFontFromFileTTF(entry.path().c_str(), 10.f, &config));
    }*/
    io.Fonts->AddFontDefault();

    auto* data = (u8*)nullptr;
    auto dimensions = std::pair<i32, i32>{};
    io.Fonts->GetTexDataAsRGBA32(&data, &dimensions.first, &dimensions.second);

    const auto* const bgfx_texture_data = bgfx::copy(data, dimensions.first * dimensions.second * 4);

    m_fonts_texture = bgfx::createTexture2D((u16)dimensions.first, (u16)dimensions.second, false, 1, bgfx::TextureFormat::BGRA8, 0, bgfx_texture_data);

    io.Fonts->TexID =
      detail::imgui_user_texture_data{
        .handle = m_fonts_texture,
        .flag_alpha_blend = true,
      }
        .to_im_texture_id();
}

void context::update_io_modifiers(glfw::window& window, ImGuiIO& io) {
    io.AddKeyEvent(ImGuiMod_Shift, window.key_is_pressed(GLFW_KEY_LEFT_SHIFT) || window.key_is_pressed(GLFW_KEY_RIGHT_SHIFT));
    io.AddKeyEvent(ImGuiMod_Ctrl, window.key_is_pressed(GLFW_KEY_LEFT_SHIFT) || window.key_is_pressed(GLFW_KEY_RIGHT_SHIFT));
    io.AddKeyEvent(ImGuiMod_Alt, window.key_is_pressed(GLFW_KEY_LEFT_ALT) || window.key_is_pressed(GLFW_KEY_RIGHT_ALT));
    io.AddKeyEvent(ImGuiMod_Super, window.key_is_pressed(GLFW_KEY_LEFT_SUPER) || window.key_is_pressed(GLFW_KEY_RIGHT_SUPER));
}

void context::set_view_transform(bgfx::ViewId view_id, const ImDrawData* draw_data) {
    const auto* const caps = bgfx::getCaps();
    float ortho[16];
    const auto x = draw_data->DisplayPos.x;
    const auto y = draw_data->DisplayPos.y;
    const auto width = draw_data->DisplaySize.x;
    const auto height = draw_data->DisplaySize.y;

    bx::mtxOrtho(ortho, x, x + width, y + height, y, 0.0f, 1000.0f, 0.0f, caps->homogeneousDepth);
    bgfx::setViewTransform(view_id, NULL, ortho);
    bgfx::setViewRect(view_id, 0, 0, uint16_t(width), uint16_t(height));
}

void image(bgfx::TextureHandle handle, float width, float height) {
    const auto user_handle = detail::imgui_user_texture_data{
      .handle = handle,
      .flag_alpha_blend = true,
    };

    ImGui::Image(user_handle.to_im_texture_id(), ImVec2(width, height));
}

}  // namespace bpt::imgui
