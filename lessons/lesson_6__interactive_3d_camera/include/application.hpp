#pragma once

#include "camera_orbit.hpp"
#include "pyramid.hpp"
#include "shader_program.hpp"

struct GLFWwindow;


class Application
{
public:
    Application();
    ~Application();

    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;

    int run();

private:
    bool initialize();
    bool create_shader();

    void render_loop();
    void render();

    glm::mat4 create_projection_matrix() const;

    void on_framebuffer_size(
        int width,
        int height
    );

    void on_mouse_button(
        int button,
        int action
    );

    void on_cursor_position(
        double x,
        double y
    );

    void on_mouse_scroll(double y_offset);

    static Application* get_application(GLFWwindow* window);

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

    void shutdown();

private:
    bool m_is_glfw_initialized;
    bool m_is_opengl_initialized;
    int m_framebuffer_width;
    int m_framebuffer_height;
    int m_mvp_location;

    OrbitCamera m_camera;
    ShaderProgram m_shader;
    Pyramid m_pyramid;
    GLFWwindow* m_window;
};