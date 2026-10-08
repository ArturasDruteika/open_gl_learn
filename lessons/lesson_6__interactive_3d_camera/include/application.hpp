#pragma once

#include "camera_orbit.hpp"
#include "pyramid.hpp"
#include "shader_program.hpp"
#include "window.hpp"


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

    void shutdown();

private:
    bool m_is_opengl_initialized;
    int m_mvp_location;

    Window m_window;
    OrbitCamera m_camera;
    ShaderProgram m_shader;
    Pyramid m_pyramid;
};