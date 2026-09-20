cmake_minimum_required(VERSION 3.24)

project(lesson_3__ebo)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

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
        OpenGL::GL
        spdlog::spdlog
)

add_dependencies(
    ${PROJECT_NAME}
    glad
)

set_target_properties(
    ${PROJECT_NAME}
    PROPERTIES
        INSTALL_RPATH "$ORIGIN/../lib"
)

# ---------------------------------------------------------------------------
# Install
# ---------------------------------------------------------------------------

install(
    TARGETS ${PROJECT_NAME}

    RUNTIME
        DESTINATION "${PROJECT_NAME}/bin"

    COMPONENT runtime
)

install(
    FILES
        "$<TARGET_FILE:glad>"

    DESTINATION
        "${PROJECT_NAME}/lib"

    COMPONENT runtime
)