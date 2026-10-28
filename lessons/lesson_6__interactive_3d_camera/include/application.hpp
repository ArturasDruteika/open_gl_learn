#pragma once


#include "camera_orbit.hpp"
#include "camera_orbit_controller.hpp"
#include "mesh_open_gl.hpp"
#include "pyramid.hpp"
#include "renderer.hpp"
#include "window.hpp"

#include <memory>


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
    CameraOrbitController m_camera_controller;
    Renderer m_renderer;

    Pyramid m_pyramid;
    std::unique_ptr<MeshOpenGL> m_pyramid_mesh;
};