#include "window.hpp"

#include "GLFW/glfw3.h"

#include "spdlog/spdlog.h"

#include <utility>


Window::Window()
    : m_window{ nullptr }
    , m_is_glfw_initialized{ false }
    , m_framebuffer_width{ 0 }
    , m_framebuffer_height{ 0 }
{
}

Window::~Window()
{
    destroy();
}

bool Window::create(
    int width,
    int height,
    const char* title
)
{
    if (m_window || m_is_glfw_initialized)
        return false;

    // GLFW
    if (!glfwInit())
    {
        spdlog::error("Failed to initialize GLFW");
        return false;
    }

    m_is_glfw_initialized = true;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Window
    m_window = glfwCreateWindow(
        width,
        height,
        title,
        nullptr,
        nullptr
    );

    if (!m_window)
    {
        spdlog::error("Failed to create GLFW window");
        destroy();
        return false;
    }

    glfwMakeContextCurrent(m_window);
    glfwSetWindowUserPointer(m_window, this);

    // Callbacks
    glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
    glfwSetMouseButtonCallback(m_window, mouse_button_callback);
    glfwSetCursorPosCallback(m_window, cursor_position_callback);
    glfwSetScrollCallback(m_window, scroll_callback);

    // Framebuffer
    glfwGetFramebufferSize(
        m_window,
        &m_framebuffer_width,
        &m_framebuffer_height
    );

    return true;
}

void Window::destroy()
{
    if (m_window)
    {
        glfwDestroyWindow(m_window);
        m_window = nullptr;
    }

    if (m_is_glfw_initialized)
    {
        glfwTerminate();
        m_is_glfw_initialized = false;
    }

    m_framebuffer_width = 0;
    m_framebuffer_height = 0;
}

bool Window::should_close() const
{
    return glfwWindowShouldClose(m_window);
}

void Window::swap_buffers() const
{
    glfwSwapBuffers(m_window);
}

void Window::poll_events() const
{
    glfwPollEvents();
}

bool Window::is_key_pressed(int key) const
{
    return glfwGetKey(m_window, key) == GLFW_PRESS;
}

int Window::get_framebuffer_width() const
{
    return m_framebuffer_width;
}

int Window::get_framebuffer_height() const
{
    return m_framebuffer_height;
}

void Window::set_framebuffer_size_callback(FramebufferSizeCallback callback)
{
    m_framebuffer_size_callback = std::move(callback);
}

void Window::set_mouse_button_callback(MouseButtonCallback callback)
{
    m_mouse_button_callback = std::move(callback);
}

void Window::set_cursor_position_callback(CursorPositionCallback callback)
{
    m_cursor_position_callback = std::move(callback);
}

void Window::set_scroll_callback(ScrollCallback callback)
{
    m_scroll_callback = std::move(callback);
}

Window* Window::get_window(GLFWwindow* window)
{
    return static_cast<Window*>(glfwGetWindowUserPointer(window));
}

void Window::framebuffer_size_callback(
    GLFWwindow* window,
    int width,
    int height
)
{
    Window* self_instance = get_window(window);

    if (self_instance)
        self_instance->on_framebuffer_size(width, height);
}

void Window::mouse_button_callback(
    GLFWwindow* window,
    int button,
    int action,
    int mods
)
{
    Window* self_instance = get_window(window);

    if (self_instance)
        self_instance->on_mouse_button(button, action);
}

void Window::cursor_position_callback(
    GLFWwindow* window,
    double x,
    double y
)
{
    Window* self_instance = get_window(window);

    if (self_instance)
        self_instance->on_cursor_position(x, y);
}

void Window::scroll_callback(
    GLFWwindow* window,
    double x_offset,
    double y_offset
)
{
    Window* self_instance = get_window(window);

    if (self_instance)
        self_instance->on_scroll(y_offset);
}

void Window::on_framebuffer_size(int width, int height)
{
    m_framebuffer_width = width;
    m_framebuffer_height = height;

    if (m_framebuffer_size_callback)
        m_framebuffer_size_callback(width, height);
}

void Window::on_mouse_button(int button, int action)
{
    if (m_mouse_button_callback)
        m_mouse_button_callback(button, action);
}

void Window::on_cursor_position(double x, double y)
{
    if (m_cursor_position_callback)
        m_cursor_position_callback(x, y);
}

void Window::on_scroll(double y_offset)
{
    if (m_scroll_callback)
        m_scroll_callback(y_offset);
}