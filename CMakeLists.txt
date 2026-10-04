cmake_minimum_required(VERSION 3.24)

project(open_gl_learn)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Prevent GLFW from adding its development files to the install target.
set(GLFW_INSTALL OFF CACHE BOOL "Disable GLFW installation" FORCE)

# ---------------------------------------------------------------------------
# Build output directories
#
# These affect only the intermediate build tree.
# The final installation layout is controlled by install() rules
# in each lesson.
# ---------------------------------------------------------------------------

if(NOT CMAKE_GENERATOR MATCHES "Visual Studio")

    set(
        CMAKE_RUNTIME_OUTPUT_DIRECTORY
        "${CMAKE_BINARY_DIR}/$<LOWER_CASE:$<CONFIG>>/bin"
    )

    set(
        CMAKE_LIBRARY_OUTPUT_DIRECTORY
        "${CMAKE_BINARY_DIR}/$<LOWER_CASE:$<CONFIG>>/lib"
    )

    set(
        CMAKE_ARCHIVE_OUTPUT_DIRECTORY
        "${CMAKE_BINARY_DIR}/$<LOWER_CASE:$<CONFIG>>/lib"
    )

    set(
        CMAKE_ASSETS_OUTPUT_DIRECTORY
        "${CMAKE_BINARY_DIR}/$<LOWER_CASE:$<CONFIG>>/res"
    )

endif()

# Dependencies
add_subdirectory(external)
add_subdirectory(andromeda)

# Lessons
add_subdirectory(lessons)