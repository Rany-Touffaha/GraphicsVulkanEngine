cmake_minimum_required(VERSION 4.1.2)
project(GraphicsVulkanEngine)

find_package(Vulkan REQUIRED)

# MoltenVK setup
if(APPLE)
    set(MOLTENVK_DYLIB "/usr/local/lib/libMoltenVK.dylib")
    set(MOLTENVK_INCLUDE_DIR "/usr/local/lib/MoltenVK.xcframework/macos-arm64_x86_64/include")
endif()

include(cmake/shaders.cmake)

include(FetchContent)

FetchContent_Declare(
    glm
    GIT_REPOSITORY "https://github.com/g-truc/glm.git"
    GIT_TAG "1.0.2"
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glm)

FetchContent_Declare(
    splog
    GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
    GIT_TAG "v1.16.0"
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(splog)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw.git"
    GIT_TAG "3.4"
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(glfw)

FetchContent_Declare(
    microsoft-gsl
    GIT_REPOSITORY "https://github.com/microsoft/GSL.git"
    GIT_TAG "v4.2.0"
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(microsoft-gsl)

file(GLOB_RECURSE VulkanEngineSources CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"

)

add_executable(GraphicsVulkanEngine ${VulkanEngineSources})

target_link_libraries(GraphicsVulkanEngine PRIVATE Vulkan::Vulkan)
target_link_libraries(GraphicsVulkanEngine PRIVATE glm)
target_link_libraries(GraphicsVulkanEngine PRIVATE glfw)
target_link_libraries(GraphicsVulkanEngine PRIVATE Microsoft.GSL::GSL)
target_link_libraries(GraphicsVulkanEngine PRIVATE spdlog::spdlog)
if(APPLE)
    target_link_libraries(GraphicsVulkanEngine PRIVATE ${MOLTENVK_DYLIB})
    target_include_directories(GraphicsVulkanEngine PRIVATE ${MOLTENVK_INCLUDE_DIR})
    find_library(METAL_FRAMEWORK Metal)
    find_library(QUARTZCORE_FRAMEWORK QuartzCore)
    find_library(IOSURFACE_FRAMEWORK IOSurface)
    find_library(COREFRAMEWORK CoreFoundation)
    target_link_libraries(GraphicsVulkanEngine PRIVATE
        ${METAL_FRAMEWORK}
        ${QUARTZCORE_FRAMEWORK}
        ${IOSURFACE_FRAMEWORK}
        ${COREFRAMEWORK}
    )
endif()

target_include_directories(GraphicsVulkanEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src")
target_include_directories(GraphicsVulkanEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/build")

target_compile_features(GraphicsVulkanEngine PRIVATE cxx_std_20)

target_precompile_headers(GraphicsVulkanEngine PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/src/precomp.h")

file(GLOB_RECURSE ShaderSources CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.vert"
    "${CMAKE_CURRENT_SOURCE_DIR}/shaders/*.frag"

)
add_shaders(VulkanEngineShaders ${ShaderSources})
add_dependencies(GraphicsVulkanEngine VulkanEngineShaders)
