#include <boilerplate/platform/platform.hpp>

#include <stuff/thread.hpp>

#include <thread>

namespace bpt::detail {

void platform::main_loop(bool(*frame_fn)(void*), void* arg) {
    auto timer_chan = stf::channel<void>{};

    std::thread timer_thread {
      [this, &timer_chan] {
          for (;;) {
              if (!stf::send(timer_chan)) {
                  break;
              }

              std::this_thread::sleep_for(std::chrono::microseconds(static_cast<uint64_t>(1e6 / m_fps.load())));
          }
      }
    };

    for (;;) {
        stf::receive(timer_chan);

        if (!frame_fn(arg)) {
            break;
        }
    }

    timer_chan.close();
    timer_thread.join();
}

}
