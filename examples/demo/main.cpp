#include <boilerplate/glfw.hpp>
#include <boilerplate/program.hpp>
#include <boilerplate/imgui.hpp>

#include <stuff/qoi.hpp>

struct example_program : bpt::program {
    example_program() {
        m_window.create({1024, 768}, "boilerplate");

        program::init_bgfx(m_window);

        bgfx::setViewClear(view_id, BGFX_CLEAR_COLOR);
        bgfx::setViewRect(view_id, 0, 0, bgfx::BackbufferRatio::Equal);

        stf::qoi::image test_image{};
        test_image.from_file("uv_grid_0.qoi");
        const auto* const texture_data = bgfx::copy((u8*)test_image.pixels().data(), test_image.width() * test_image.height() * 4);
        m_test_image = bgfx::createTexture2D(test_image.width(), test_image.height(), false, 1, bgfx::TextureFormat::RGBA8, 0, texture_data);

        m_imgui_context.init(m_window);
    }

    auto frame() -> bool override {
        if (!m_window.open()) {
            return false;
        }

        m_imgui_context.new_frame(m_window);

        auto context_guard = m_imgui_context.make_current_context_guarded();

        m_window.poll_events();
        for (;;) {
            stf::multi_visitor event_visitor{
              [](bpt::glfw::event::window_size event) {
                  bgfx::reset((u32)event.size[0], (u32)event.size[1], BGFX_RESET_NONE);
                  bgfx::setViewRect(view_id, 0, 0, bgfx::BackbufferRatio::Equal);  //
              },
              [](auto const&) {},
            };

            auto event = TRY_OR_BREAK(m_window.pop_event());

            if (m_imgui_context.process_event(m_window, event)) {
                break;
            }

            std::visit(event_visitor, event);
        }

        bgfx::touch(view_id);

        ImGui::Begin("asd");
        if (ImGui::Button("No Debug View")) {
            bgfx::setDebug(BGFX_DEBUG_NONE);
        }
        ImGui::SameLine();
        if (ImGui::Button("Show Debug Stats")) {
            bgfx::setDebug(BGFX_DEBUG_STATS);
        }
        ImGui::SameLine();
        if (ImGui::Button("Show Debug Text")) {
            bgfx::setDebug(BGFX_DEBUG_TEXT);
        }

        if (ImGui::Button("asdf")) {
            std::ignore = 0;
        }

        bpt::imgui::image(m_test_image, 512, 512);
        ImGui::End();

        ImGui::ShowDemoWindow();
        ImGui::ShowMetricsWindow();

        m_imgui_context.render(m_window);
        bgfx::frame();

        return true;
    }

private:
    static constexpr bgfx::ViewId view_id = 0;
    bpt::glfw::window m_window {};
    bpt::imgui::context m_imgui_context {};
    bgfx::TextureHandle m_test_image {bgfx::kInvalidHandle};
};

int main(int argc, char** argv, char** env) {
    return bpt::run(new example_program(), argc, argv, env);
}
