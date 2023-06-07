#include <boilerplate/platform/emscripten.hpp>

#ifdef __EMSCRIPTEN__

extern "C" const char* __asan_default_options() { return "detect_leaks=0"; }

#include <emscripten/emscripten.h>
#include <emscripten/html5.h>

namespace bpt::detail {
 
void platform_emscripten::fps_hint(double fps) {
    platform::fps_hint(fps);
}

void platform_emscripten::main_loop(bool (*frame_fn)(void*), void* frame_fn_arg) {
    m_frame_fn = frame_fn;
    m_frame_fn_arg = frame_fn_arg;

    const auto frame_fn_emscripten = [](double, void* arg) -> EM_BOOL {
        auto& self = *reinterpret_cast<platform_emscripten*>(arg);
        return self.m_frame_fn(self.m_frame_fn_arg) ? EM_TRUE : EM_FALSE;
    };

    emscripten_request_animation_frame_loop(frame_fn_emscripten, reinterpret_cast<void*>(this));
}

}// namespace bpt::detail

#endif
