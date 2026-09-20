cmake_minimum_required(VERSION 3.24)

project(lesson_5__going_to_3d)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ---------------------------------------------------------------------------
# Executable
# ---------------------------------------------------------------------------

add_executable(
    ${PROJECT_NAME}
    main.cpp
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
        utils
)

add_dependencies(
    ${PROJECT_NAME}
    glad
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
        "${PROJECT_NAME}/bin/shader_program_sources"

    COMPONENT runtime
)