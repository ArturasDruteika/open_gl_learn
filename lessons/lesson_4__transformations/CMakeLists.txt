cmake_minimum_required(VERSION 3.24)

project(lesson_4__transformations)

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
        andromeda::utils
)

add_dependencies(
    ${PROJECT_NAME}
    glad
)

# ---------------------------------------------------------------------------
# Runtime RPATH
#
# executable:
#   lesson_4__transformations/bin/lesson_4__transformations
#
# libraries:
#   lesson_4__transformations/lib/
#
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