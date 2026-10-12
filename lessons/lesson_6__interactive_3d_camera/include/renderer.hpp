#pragma once


#include "camera_orbit.hpp"
#include "mesh_open_gl.hpp"
#include "shader_program.hpp"

#include <glm/glm.hpp>


class Renderer
{
public:
    Renderer();
    ~Renderer();

    Renderer(const Renderer&) = delete;
    Renderer& operator=(const Renderer&) = delete;

    bool initialize();
    void shutdown();

    void set_viewport(int width,
                      int height);

    void render(const OrbitCamera& camera,
                const MeshOpenGL& mesh,
                const glm::mat4& model,
                int width,
                int height);

private:
    bool create_shader();

    glm::mat4 create_projection_matrix(int width,
                                       int height) const;

private:
    bool m_is_initialized;
    int m_mvp_location;

    ShaderProgram m_shader;
};