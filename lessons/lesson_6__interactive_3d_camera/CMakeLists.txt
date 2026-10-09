cmake_minimum_required(VERSION 3.24)

project(lesson_6__interactive_3d_camera)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(
    ${PROJECT_NAME} 
    include/application.hpp
    include/camera_orbit_controller.hpp
    include/camera_orbit.hpp
    include/mesh.hpp
    include/pyramid.hpp
    include/renderer.hpp
    include/shader_program.hpp
    include/vertex.hpp
    include/window.hpp
    src/application.cpp
    src/camera_orbit_controller.cpp
    src/camera_orbit.cpp
    src/mesh.cpp
    src/pyramid.cpp
    src/renderer.cpp
    src/shader_program.cpp
    src/window.cpp
    src/main.cpp
)

# ---------------------------------------------------------------------------
# OpenGL
# ---------------------------------------------------------------------------

find_package(OpenGL REQUIRED)

if(APPLE)
    target_compile_definitions(
        ${PROJECT_NAME}
        PRIVATE
            GL_SILENCE_DEPRECATION
    )
endif()

# ---------------------------------------------------------------------------
# Libraries
# ---------------------------------------------------------------------------

target_link_libraries(
    ${PROJECT_NAME}

    PRIVATE
        glad
        glfw
        glm::glm
        OpenGL::GL
        spdlog::spdlog

    PUBLIC
        andromeda::utils
)

add_dependencies(
    ${PROJECT_NAME}
    glad
)

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

# ---------------------------------------------------------------------------
# Runtime RPATH
#
# Installed layout:
#
# lesson_5__going_to_3d/
# ├── bin/
# │   ├── lesson_5__going_to_3d
# │   └── shader_program_sources/
# │       ├── vertex.glsl
# │       └── fragment.glsl
# └── lib/
#     └── libglad.so
#
# $ORIGIN is the directory containing the executable (bin/).
# Therefore $ORIGIN/../lib points to this lesson's lib/ directory.
# ---------------------------------------------------------------------------

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        INSTALL_RPATH "$ORIGIN/../lib"
)

# ---------------------------------------------------------------------------
# Shaders
# ---------------------------------------------------------------------------

set(
    SHADER_SOURCE_DIR
    "${CMAKE_CURRENT_SOURCE_DIR}/shader_program_sources"
)

# Detect missing shaders during CMake configuration instead of failing
# later during the build/install.
if(NOT EXISTS "${SHADER_SOURCE_DIR}/vertex.glsl")
    message(
        FATAL_ERROR
        "Missing shader: ${SHADER_SOURCE_DIR}/vertex.glsl"
    )
endif()

if(NOT EXISTS "${SHADER_SOURCE_DIR}/fragment.glsl")
    message(
        FATAL_ERROR
        "Missing shader: ${SHADER_SOURCE_DIR}/fragment.glsl"
    )
endif()

# ---------------------------------------------------------------------------
# Copy resources to build directory
#
# build/linux_clang_release/
# └── release/
#     └── res/
#         └── shader_program_sources/
# ---------------------------------------------------------------------------

add_custom_command(
    TARGET ${PROJECT_NAME}
    POST_BUILD

    COMMAND
        ${CMAKE_COMMAND} -E make_directory
        "${CMAKE_ASSETS_OUTPUT_DIRECTORY}/shader_program_sources"

    COMMAND
        ${CMAKE_COMMAND} -E copy_directory
        "${SHADER_SOURCE_DIR}"
        "${CMAKE_ASSETS_OUTPUT_DIRECTORY}/shader_program_sources"

    COMMENT
        "Copying shader resources to res/"
)

# ---------------------------------------------------------------------------
# Install executable
# ---------------------------------------------------------------------------

install(
    TARGETS ${PROJECT_NAME}

    RUNTIME
        DESTINATION "${PROJECT_NAME}/bin"

    COMPONENT runtime
)

# ---------------------------------------------------------------------------
# Install shared runtime libraries
# ---------------------------------------------------------------------------

install(
    FILES
        "$<TARGET_FILE:glad>"

    DESTINATION
        "${PROJECT_NAME}/lib"

    COMPONENT runtime
)

# ---------------------------------------------------------------------------
# Install lesson-specific shaders
# ---------------------------------------------------------------------------

install(
    DIRECTORY
        "${SHADER_SOURCE_DIR}/"

    DESTINATION
        "${PROJECT_NAME}/res/shader_program_sources"

    COMPONENT runtime
)