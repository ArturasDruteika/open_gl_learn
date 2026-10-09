#pragma once


#include "camera_orbit.hpp"
#include "camera_orbit_controller.hpp"
#include "renderer.hpp"
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
    void register_callbacks();
    void render_loop();
    void shutdown();

private:
    bool m_is_opengl_initialized;

    Window m_window;
    OrbitCamera m_camera;
    OrbitCameraController m_camera_controller;
    Renderer m_renderer;
};