#include "application.hpp"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "glm/gtc/matrix_transform.hpp"

#include "spdlog/spdlog.h"

#include <memory>


// Constants
namespace
{
    constexpr int WINDOW_WIDTH = 800;
    constexpr int WINDOW_HEIGHT = 800;
}


Application::Application()
    : m_is_opengl_initialized{ false }
    , m_camera{ glm::vec3(0.0f, 0.0f, 2.5f),
                glm::radians(20.0f),
                glm::radians(-35.0f),
                glm::radians(90.0f) }
    , m_camera_controller{ m_camera }
    , m_pyramid{ glm::vec3{ 0.0f, 0.0f, 0.0f } }
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
    if (!m_window.create(WINDOW_WIDTH,
                         WINDOW_HEIGHT,
                         "3D Pyramid"))
    {
        return false;
    }

    // GLAD
    if (!gladLoadGL(glfwGetProcAddress))
    {
        spdlog::error("Failed to initialize GLAD");
        return false;
    }

    m_is_opengl_initialized = true;

    // Renderer
    if (!m_renderer.initialize())
        return false;

    // Pyramid
    m_pyramid.set_scale(glm::vec3{ 1.4f });

    m_pyramid_mesh = std::make_unique<MeshOpenGL>(m_pyramid.get_mesh());

    // Framebuffer
    m_renderer.set_viewport(m_window.get_framebuffer_width(),
                            m_window.get_framebuffer_height());

    // Callbacks
    register_callbacks();

    return true;
}

void Application::register_callbacks()
{
    // Framebuffer
    m_window.set_framebuffer_size_callback(
        [this](int width, int height)
        {
            m_renderer.set_viewport(width, height);
        }
    );

    // Mouse button
    m_window.set_mouse_button_callback(
        [this](int button, int action)
        {
            m_camera_controller.on_mouse_button(button,
                                                action);
        }
    );

    // Cursor position
    m_window.set_cursor_position_callback(
        [this](double x, double y)
        {
            const bool is_ctrl_down =
                m_window.is_key_pressed(GLFW_KEY_LEFT_CONTROL) ||
                m_window.is_key_pressed(GLFW_KEY_RIGHT_CONTROL);

            m_camera_controller.on_cursor_position(x,
                                                   y,
                                                   is_ctrl_down);
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

void Application::render_loop()
{
    while (!m_window.should_close())
    {
        const int width = m_window.get_framebuffer_width();
        const int height = m_window.get_framebuffer_height();

        if (width > 0 && height > 0)
        {
            m_renderer.render(m_camera,
                              *m_pyramid_mesh,
                              m_pyramid.get_model_matrix(),
                              width,
                              height);

            m_window.swap_buffers();
        }

        m_window.poll_events();
    }
}

void Application::shutdown()
{
    // OpenGL resources must be destroyed while the OpenGL context still exists.
    if (m_is_opengl_initialized)
    {
        m_pyramid_mesh.reset();
        m_renderer.shutdown();

        m_is_opengl_initialized = false;
    }

    // Window / OpenGL context
    m_window.destroy();
}