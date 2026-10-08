#pragma once

#include "camera_orbit.hpp"
#include "camera_orbit_controller.hpp"
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

    void register_callbacks();

    void render_loop();
    void render();

    glm::mat4 create_projection_matrix() const;

    void shutdown();

private:
    bool m_is_opengl_initialized;
    int m_mvp_location;

    Window m_window;

    OrbitCamera m_camera;
    OrbitCameraController m_camera_controller;

    ShaderProgram m_shader;
    Pyramid m_pyramid;
};