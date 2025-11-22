cmake_minimum_required(VERSION 4.1.2)
project(GraphicsVulkanEngine)

find_package(Vulkan REQUIRED)

# MoltenVK setup
if(APPLE)
    find_library(MOLTENVK_LIBRARY MoltenVK REQUIRED)
    get_filename_component(MOLTENVK_LIB_DIR "${MOLTENVK_LIBRARY}" DIRECTORY)
    get_filename_component(MOLTENVK_DIR "${MOLTENVK_LIB_DIR}/../.." ABSOLUTE)
    set(MOLTENVK_INCLUDE_DIR "${MOLTENVK_DIR}/include")
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
    target_link_libraries(GraphicsVulkanEngine PRIVATE ${MOLTENVK_LIBRARY})
    target_include_directories(GraphicsVulkanEngine PRIVATE ${MOLTENVK_INCLUDE_DIR})
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
