#pragma once

#include <stuff/core.hpp>
#include <stuff/expected.hpp>

#include <bgfx/bgfx.h>
#include <fmt/format.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <functional>
#include <vector>

namespace bpt {

enum struct shader_type {
    vertex,
    fragment,
    compute,
};

auto load_shader(std::filesystem::path const& path, std::string_view name, shader_type type) -> stf::expected<bgfx::ShaderHandle, std::string_view> {
    constexpr auto renderer_extension = [](bgfx::RendererType::Enum type) constexpr->std::string_view {
        switch (type) {
            case bgfx::RendererType::Noop: return "noop";
            case bgfx::RendererType::Agc: return "agc";
            case bgfx::RendererType::Direct3D9: return "d3d9";
            case bgfx::RendererType::Direct3D11: return "d3d11";
            case bgfx::RendererType::Direct3D12: return "d3d12";
            case bgfx::RendererType::Gnm: return "gnm";
            case bgfx::RendererType::Metal: return "metal";
            case bgfx::RendererType::Nvn: return "nvn";
            case bgfx::RendererType::OpenGLES: return "essl";
            case bgfx::RendererType::OpenGL: return "glsl";
            case bgfx::RendererType::Vulkan: return "vulkan";
            case bgfx::RendererType::WebGPU: return "essl_wgpu";
            default: std::unreachable();
        }
    };

    constexpr auto shader_extension = [](shader_type type) constexpr->std::string_view {
        switch (type) {
            case shader_type::vertex: return "vs";
            case shader_type::fragment: return "fs";
            case shader_type::compute: return "cs";
            default: std::unreachable();
        }
    };

    const auto* const caps = bgfx::getCaps();
    const auto renderer_type = caps->rendererType;

    const auto extension = renderer_extension(renderer_type);

    const auto cur_path = path / fmt::format("{}.{}.{}.bin", name, extension, shader_extension(type));
    auto ifs = std::ifstream(cur_path);

    if (!ifs) {
        return stf::unexpected{ "unable to open requested file" };
    }

    auto file_contents = std::vector<u8>{};
    std::copy(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>(), std::back_inserter(file_contents));
    return bgfx::createShader(bgfx::copy(file_contents.data(), file_contents.size()));
}

}  // namespace bpt
