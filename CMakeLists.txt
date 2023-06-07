cmake_minimum_required(VERSION 3.26)
project(boilerplate)

set(CMAKE_CXX_STANDARD 23)

if (EMSCRIPTEN)
    message("emscripten path: ${EMSCRIPTEN}")
    set(GLFW_BUILD_X11 OFF)
    add_compile_definitions(BX_CONFIG_SUPPORTS_THREADING=1)
    set(BGFX_CONFIG_MULTITHREADED ON)
    set(CMAKE_TOOLCHAIN_FILE ${EMSCRIPTEN}/cmake/Modules/Platform/Emscripten.cmake)

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sINITIAL_MEMORY=536870912")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sPTHREAD_POOL_SIZE=16")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sNO_EXIT_RUNTIME=1")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -sALLOW_MEMORY_GROWTH")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -sUSE_GLFW=3")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --preload-file ${CMAKE_SOURCE_DIR}/run@/")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-unused-command-line-argument -pthread")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wno-unused-command-line-argument -pthread")
    set(CMAKE_EXECUTABLE_SUFFIX .html)
else ()
    message("not an emscripten build")
    add_subdirectory(thirdparty/glfw)
endif ()

if (CMAKE_BUILD_TYPE STREQUAL "Debug" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fsanitize=undefined")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address -fsanitize=undefined")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math -march=native -mtune=native -fopenmp")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fopenmp")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -flto")

add_subdirectory(thirdparty/bgfx.cmake)
add_subdirectory(thirdparty/fmt)
add_subdirectory(thirdparty/stuff/libs/blas)
add_subdirectory(thirdparty/stuff/libs/expected)
add_subdirectory(thirdparty/stuff/libs/thread)

add_library(${PROJECT_NAME}
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
        inc)

### DEMO

add_subdirectory(thirdparty/stuff/libs/qoi)

add_executable(${PROJECT_NAME}_demo
        examples/demo/main.cpp)

target_link_libraries(${PROJECT_NAME}_demo ${PROJECT_NAME} stuff_qoi)
