#pragma once


#include "camera_orbit.hpp"
#include "mesh.hpp"
#include "pyramid.hpp"
#include "shader_program.hpp"


class Renderer
{
public:
    Renderer();
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    bool initialize();
    void shutdown();

    void set_viewport(int width, int height);
    void render(const OrbitCamera& camera, int width, int height);

private:
    bool create_shader();

    glm::mat4 create_projection_matrix(int width, int height) const;

private:
    bool m_is_initialized;
    int m_mvp_location;

    ShaderProgram m_shader;
    Pyramid m_pyramid;
    Mesh m_mesh;
};