#include <boilerplate/platform/platform.hpp>

#include <stuff/thread.hpp>

#include <thread>

namespace bpt::detail {

void platform::main_loop(bool(*frame_fn)(void*), void* arg) {
    auto prev_frame = std::chrono::steady_clock::now();

    for (;;) {
        const auto frame_duration = std::chrono::microseconds(static_cast<long long>(1'000'000. / m_fps.load()));
        const auto next_frame = prev_frame + frame_duration;
        prev_frame = next_frame;
        std::this_thread::sleep_until(next_frame);

        if (!frame_fn(arg)) {
            break;
        }
    }
}

}
