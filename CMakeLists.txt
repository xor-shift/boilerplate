cmake_minimum_required(VERSION 3.26)
project(boilerplate)

set(CMAKE_CXX_STANDARD 23)

if (EMSCRIPTEN)
    message("emscripten path: ${EMSCRIPTEN}")
    set(GLFW_BUILD_X11 OFF)
    add_compile_definitions(BX_CONFIG_SUPPORTS_THREADING=1)
    set(BGFX_CONFIG_MULTITHREADED ON)
    set(CMAKE_TOOLCHAIN_FILE ${EMSCRIPTEN}/cmake/Modules/Platform/Emscripten.cmake)

    add_link_options(-sINITIAL_MEMORY=536870912)
    add_link_options(-sPTHREAD_POOL_SIZE=16)
    add_link_options(-sNO_EXIT_RUNTIME=1)
    #add_compile_options(-sALLOW_MEMORY_GROWTH)
    add_link_options(-sUSE_GLFW=3)

    add_compile_options(-Wno-unused-command-line-argument -pthread)
    add_link_options(-Wno-unused-command-line-argument -pthread)
    #set(CMAKE_EXECUTABLE_SUFFIX .html)
else ()
    message("not an emscripten build")
    add_subdirectory(thirdparty/glfw)
endif ()

add_compile_options(-stdlib=libc++)
add_link_options(-stdlib=libc++)

add_subdirectory(thirdparty/bgfx.cmake)
add_subdirectory(thirdparty/fmt)
add_subdirectory(thirdparty/stuff/libs/blas)
add_subdirectory(thirdparty/stuff/libs/expected)
add_subdirectory(thirdparty/stuff/libs/thread)

### SHADERS

bgfx_compile_shader_to_header(
        TYPE VERTEX
        SHADERS ${CMAKE_SOURCE_DIR}/src/boilerplate/shaders/imgui/imgui.vs.sc
        VARYING_DEF ${CMAKE_SOURCE_DIR}/src/boilerplate/shaders/imgui/varying.def.sc
        OUTPUT_DIR ${CMAKE_BINARY_DIR}/inc/shaders/
        INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/thirdparty/bgfx.cmake/bgfx/src/
)

bgfx_compile_shader_to_header(
        TYPE FRAGMENT
        SHADERS ${CMAKE_SOURCE_DIR}/src/boilerplate/shaders/imgui/imgui.fs.sc
        VARYING_DEF ${CMAKE_SOURCE_DIR}/src/boilerplate/shaders/imgui/varying.def.sc
        OUTPUT_DIR ${CMAKE_BINARY_DIR}/inc/shaders/
        INCLUDE_DIRS ${CMAKE_SOURCE_DIR}/thirdparty/bgfx.cmake/bgfx/src/
)

### LIBRARY

add_library(${PROJECT_NAME}
        #${CMAKE_SOURCE_DIR}/src/boilerplate/shaders/imgui/imgui.vs.sc
        #${CMAKE_SOURCE_DIR}/src/boilerplate/shaders/imgui/imgui.fs.sc

        thirdparty/imgui/imgui.cpp
        thirdparty/imgui/imgui_draw.cpp
        thirdparty/imgui/imgui_tables.cpp
        thirdparty/imgui/imgui_widgets.cpp
        thirdparty/imgui/imgui_demo.cpp

        src/boilerplate/glfw.cpp
        src/boilerplate/imgui.cpp
        src/boilerplate/program.cpp

        src/boilerplate/platform/emscripten.cpp
        src/boilerplate/platform/linux.cpp
        src/boilerplate/platform/platform.cpp
        )
target_link_libraries(${PROJECT_NAME}
        fmt::fmt
        glfw bx bimg bgfx
        stuff_blas stuff_expected stuff_thread
        )
target_include_directories(${PROJECT_NAME} PUBLIC
        thirdparty/imgui
        ${CMAKE_BINARY_DIR}/inc/
        inc)

if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    #target_compile_options(${PROJECT_NAME} PUBLIC -fsanitize=address -fsanitize=undefined)
    #target_link_options(${PROJECT_NAME} PUBLIC -fsanitize=address -fsanitize=undefined)
    add_compile_options(-fsanitize=address -fsanitize=undefined)
    add_link_options(-fsanitize=address -fsanitize=undefined)
elseif (CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    add_compile_options(-ffast-math -march=native -mtune=native -fopenmp -flto)
    add_link_options(-fopenmp -flto)
endif ()

### DEMO

add_subdirectory(thirdparty/stuff/libs/qoi)

add_executable(${PROJECT_NAME}_demo
        examples/demo/main.cpp)

if (EMSCRIPTEN)
    set_target_properties(${PROJECT_NAME}_demo PROPERTIES EXECUTABLE_SUFFIX .html)
    target_link_options(${PROJECT_NAME}_demo PRIVATE --preload-file ${CMAKE_SOURCE_DIR}/run@/)
    target_link_options(shaderc PRIVATE --embed-file ${CMAKE_SOURCE_DIR}/src/boilerplate/shaders/)
endif ()

target_link_libraries(${PROJECT_NAME}_demo ${PROJECT_NAME} stuff_qoi)
