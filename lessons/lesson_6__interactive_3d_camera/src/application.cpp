#include "application.hpp"

#include "andromeda/utils/file_operations/file_operations.hpp"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "spdlog/spdlog.h"

#include <exception>
#include <filesystem>
#include <string>


// Constants
namespace
{
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 800;

    constexpr float BACKGROUND_COLOR[4] =
    {
        0.1f,
        0.2f,
        0.3f,
        1.0f
    };

    constexpr float FOV_DEGREES_Y_AXIS = 45.0f;
    constexpr float NEAR_PLANE = 0.1f;
    constexpr float FAR_PLANE = 100.0f;
}


Application::Application()
    : m_is_opengl_initialized{ false }
    , m_mvp_location{ -1 }
    , m_camera{
        glm::vec3(0.0f, 0.0f, 2.5f),
        glm::radians(20.0f),
        glm::radians(-35.0f),
        glm::radians(90.0f)
    }
    , m_camera_controller{ m_camera }
{
}

Application::~Application()
{
    shutdown();
}

int Application::run()
{
    if (!initialize())
    {
        shutdown();
        return -1;
    }

    render_loop();
    shutdown();

    return 0;
}

bool Application::initialize()
{
    // Window
    if (!m_window.create(
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        "3D Pyramid"
    ))
    {
        return false;
    }

    // Callbacks
    register_callbacks();

    // GLAD
    if (!gladLoadGL(glfwGetProcAddress))
    {
        spdlog::error("Failed to initialize GLAD");
        return false;
    }

    m_is_opengl_initialized = true;

    // Framebuffer
    glViewport(
        0,
        0,
        m_window.get_framebuffer_width(),
        m_window.get_framebuffer_height()
    );

    // OpenGL state
    glEnable(GL_DEPTH_TEST);

    // Geometry
    m_pyramid.create();

    // Shader
    if (!create_shader())
        return false;

    // Uniforms
    m_mvp_location = m_shader.get_uniform_location("u_mvp");

    if (m_mvp_location == -1)
    {
        spdlog::error("Failed to find uniform location for u_mvp");
        return false;
    }

    return true;
}

void Application::register_callbacks()
{
    // Framebuffer
    m_window.set_framebuffer_size_callback(
        [](int width, int height)
        {
            glViewport(0, 0, width, height);
        }
    );

    // Mouse button
    m_window.set_mouse_button_callback(
        [this](int button, int action)
        {
            m_camera_controller.on_mouse_button(
                button,
                action
            );
        }
    );

    // Cursor position
    m_window.set_cursor_position_callback(
        [this](double x, double y)
        {
            const bool is_ctrl_down =
                m_window.is_key_pressed(GLFW_KEY_LEFT_CONTROL) ||
                m_window.is_key_pressed(GLFW_KEY_RIGHT_CONTROL);

            m_camera_controller.on_cursor_position(
                x,
                y,
                is_ctrl_down
            );
        }
    );

    // Mouse scroll
    m_window.set_scroll_callback(
        [this](double y_offset)
        {
            m_camera_controller.on_mouse_scroll(y_offset);
        }
    );
}

bool Application::create_shader()
{
    const std::filesystem::path vertex_shader_path = "../res/shader_program_sources/vertex.glsl";
    const std::filesystem::path fragment_shader_path = "../res/shader_program_sources/fragment.glsl";

    std::string vertex_shader_source;
    std::string fragment_shader_source;

    try
    {
        vertex_shader_source = andromeda::utils::FileOperations::load_file_as_string(vertex_shader_path);
        fragment_shader_source = andromeda::utils::FileOperations::load_file_as_string(fragment_shader_path);
    }
    catch (const std::exception& exception)
    {
        spdlog::error("Failed to load shader files: {}", exception.what());
        return false;
    }

    return m_shader.create(
        vertex_shader_source,
        fragment_shader_source
    );
}

void Application::render_loop()
{
    while (!m_window.should_close())
    {
        if (m_window.get_framebuffer_width() > 0 &&
            m_window.get_framebuffer_height() > 0)
        {
            render();
            m_window.swap_buffers();
        }

        m_window.poll_events();
    }
}

void Application::render()
{
    // Clear
    glClearColor(
        BACKGROUND_COLOR[0],
        BACKGROUND_COLOR[1],
        BACKGROUND_COLOR[2],
        BACKGROUND_COLOR[3]
    );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Projection
    const glm::mat4 projection = create_projection_matrix();

    // View
    const glm::mat4 view = m_camera.get_view_matrix();

    // Model
    const glm::mat4 model = glm::scale(
        glm::mat4(1.0f),
        glm::vec3(1.4f, 1.4f, 1.4f)
    );

    // MVP
    const glm::mat4 mvp = projection * view * model;

    // Shader
    m_shader.use();
    glUniformMatrix4fv(m_mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

    // Geometry
    m_pyramid.draw();
}

glm::mat4 Application::create_projection_matrix() const
{
    const int width = m_window.get_framebuffer_width();
    const int height = m_window.get_framebuffer_height();

    const float aspect_ratio =
        static_cast<float>(width) /
        static_cast<float>(height);

    return glm::perspective(
        glm::radians(FOV_DEGREES_Y_AXIS),
        aspect_ratio,
        NEAR_PLANE,
        FAR_PLANE
    );
}

void Application::shutdown()
{
    // OpenGL resources must be destroyed while the OpenGL context still exists.
    if (m_is_opengl_initialized)
    {
        m_pyramid.destroy();
        m_shader.destroy();

        m_is_opengl_initialized = false;
    }

    // Window / OpenGL context
    m_window.destroy();
}