#pragma once


#include <functional>


struct GLFWwindow;


class Window
{
public:
    using FramebufferSizeCallback = std::function<void(int, int)>;
    using MouseButtonCallback = std::function<void(int, int)>;
    using CursorPositionCallback = std::function<void(double, double)>;
    using ScrollCallback = std::function<void(double)>;

    Window();
    ~Window();

    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    bool create(
        int width,
        int height,
        const char* title
    );

    void destroy();

    bool should_close() const;
    void swap_buffers() const;
    void poll_events() const;

    bool is_key_pressed(int key) const;

    int get_framebuffer_width() const;
    int get_framebuffer_height() const;

    void set_framebuffer_size_callback(FramebufferSizeCallback callback);
    void set_mouse_button_callback(MouseButtonCallback callback);
    void set_cursor_position_callback(CursorPositionCallback callback);
    void set_scroll_callback(ScrollCallback callback);

private:
    static Window* get_window(GLFWwindow* window);

    static void framebuffer_size_callback(
        GLFWwindow* window,
        int width,
        int height
    );

    static void mouse_button_callback(
        GLFWwindow* window,
        int button,
        int action,
        int mods
    );

    static void cursor_position_callback(
        GLFWwindow* window,
        double x,
        double y
    );

    static void scroll_callback(
        GLFWwindow* window,
        double x_offset,
        double y_offset
    );

    void on_framebuffer_size(int width, int height);
    void on_mouse_button(int button, int action);
    void on_cursor_position(double x, double y);
    void on_scroll(double y_offset);

private:
    bool m_is_glfw_initialized;
    int m_framebuffer_width;
    int m_framebuffer_height;

    FramebufferSizeCallback m_framebuffer_size_callback;
    MouseButtonCallback m_mouse_button_callback;
    CursorPositionCallback m_cursor_position_callback;
    ScrollCallback m_scroll_callback;
    GLFWwindow* m_window;
};