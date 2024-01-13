cmake_minimum_required(VERSION 3.21.3)

project(
    vulkan-learnings
    VERSION 1.0
    DESCRIPTION "Initial learnings with the Vulkan graphics API."
    LANGUAGES CXX)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2")

set(UTILS_SOURCE_SET
    src/utils/misc/logging.cpp
    src/utils/misc/string.cpp
    src/utils/glfw/window.cpp
    src/utils/vulkan/instance.cpp
    src/utils/vulkan/debug.cpp
    src/utils/vulkan/physical_device.cpp
    src/utils/vulkan/device.cpp
    src/utils/vulkan/helpers.cpp
    src/utils/vulkan/queue.cpp
    src/utils/vulkan/surface.cpp
    src/utils/vulkan/swap_chain.cpp
    src/utils/vulkan/image.cpp
    src/utils/vulkan/image_view.cpp)

set(MAIN_SOURCE_SET
    ${UTILS_SOURCE_SET}
    src/main.cpp)

set(TEST_SOURCE_SET
    test/testmain.cpp)

add_executable(main ${MAIN_SOURCE_SET})
target_include_directories(main PRIVATE src)
target_link_libraries(main -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi)
set_property(TARGET main PROPERTY CXX_STANDARD 17)

add_executable(test ${TEST_SOURCE_SET})
target_include_directories(test PRIVATE src)
set_property(TARGET test PROPERTY CXX_STANDARD 17)
