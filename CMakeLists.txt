cmake_minimum_required(VERSION 3.21.3)

project(
    vulkan-learnings
    VERSION 1.0
    DESCRIPTION "Initial learnings with the Vulkan graphics API."
    LANGUAGES CXX)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

find_package(Vulkan REQUIRED COMPONENTS glslc)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)

set(UTILS_VULKAN_SOURCE_SET
    src/utils/vulkan/instance.cpp
    src/utils/vulkan/debug.cpp
    src/utils/vulkan/physical_device.cpp
    src/utils/vulkan/device.cpp
    src/utils/vulkan/helpers.cpp
    src/utils/vulkan/queue.cpp
    src/utils/vulkan/surface.cpp
    src/utils/vulkan/swap_chain.cpp
    src/utils/vulkan/image.cpp
    src/utils/vulkan/image_view.cpp
    src/utils/vulkan/shader_module.cpp
    src/utils/vulkan/pipeline_layout.cpp
    src/utils/vulkan/render_pass.cpp
    src/utils/vulkan/graphics_pipeline.cpp
    src/utils/vulkan/frame_buffer.cpp
    src/utils/vulkan/command_pool.cpp
    src/utils/vulkan/command_buffer.cpp
    src/utils/vulkan/semaphore.cpp
    src/utils/vulkan/fence.cpp)

set(UTILS_MISC_SOURCE_SET
    src/utils/misc/logging.cpp
    src/utils/misc/string.cpp
    src/utils/misc/file.cpp)

set(UTILS_GLFW_SOURCE_SET
    src/utils/glfw/window.cpp)

set(MAIN_SOURCE_SET
    ${UTILS_VULKAN_SOURCE_SET}
    ${UTILS_MISC_SOURCE_SET}
    ${UTILS_GLFW_SOURCE_SET}
    src/main.cpp)

set(SHADER_SOURCE_SET
    data/shaders/triangle/fragment.frag
    data/shaders/triangle/vertex.vert)

foreach(GLSL ${SHADER_SOURCE_SET})
    get_filename_component(GLSL_DIR ${GLSL} DIRECTORY)
    get_filename_component(GLSL_FILENAME ${GLSL} NAME_WLE)
    set(SPIRV ${PROJECT_BINARY_DIR}/${GLSL_DIR}/${GLSL_FILENAME}.spv)
    get_filename_component(SPIRV_DIR ${SPIRV} DIRECTORY)
    add_custom_command(
        OUTPUT ${SPIRV}
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMAND ${CMAKE_COMMAND} -E make_directory ${SPIRV_DIR}
        COMMAND ${glslc_executable} "${GLSL}" -o ${SPIRV}
        DEPENDS ${GLSL})
    list(APPEND SPIRV_FILES ${SPIRV})
endforeach(GLSL)

add_executable(main ${MAIN_SOURCE_SET})
target_include_directories(main PRIVATE src)
target_link_libraries(main -lglfw -lvulkan -ldl -lpthread -lX11 -lXxf86vm -lXrandr -lXi)
set_property(TARGET main PROPERTY CXX_STANDARD 17)

add_custom_target(shaders DEPENDS ${SPIRV_FILES})
add_dependencies(main shaders)

set(TEST_SOURCE_SET
    test/testmain.cpp)

add_executable(test ${TEST_SOURCE_SET})
target_include_directories(test PRIVATE src)
set_property(TARGET test PROPERTY CXX_STANDARD 17)
