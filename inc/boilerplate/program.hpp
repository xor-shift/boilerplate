#pragma once

#include <boilerplate/glfw.hpp>
#include <boilerplate/imgui.hpp>

#include <bgfx/bgfx.h>

#include <memory>
#include <utility>

struct GLFWwindow;

namespace bpt {

struct program {
    program() = default;
    program(program const&) = delete;
    program(program&&) = delete;

    virtual ~program() = default;

    void init_bgfx(glfw::window& window);

    int main(int argc, char** argv, char** env);

    virtual auto frame() -> bool = 0;
};

int run(program* program, int argc, char** argv, char** env);

}
