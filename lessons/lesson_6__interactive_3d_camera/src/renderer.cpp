#include "renderer.hpp"

#include "andromeda/utils/file_operations/file_operations.hpp"

#include "glad/gl.h"

#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "spdlog/spdlog.h"

#include <exception>
#include <filesystem>
#include <string>


// Constants
namespace
{
    constexpr float BACKGROUND_COLOR[4] =
    {
        0.1f,
        0.2f,
        0.3f,
        1.0f
    };

    constexpr float FOV_DEGREES_Y_AXIS = 45.0f;
    constexpr float NEAR_PLANE = 0.1f;
    constexpr float FAR_PLANE = 100.0f;
}


Renderer::Renderer()
    : m_is_initialized{ false }
    , m_mvp_location{ -1 }
{
}

Renderer::~Renderer()
{
    shutdown();
}

bool Renderer::initialize()
{
    if (m_is_initialized)
        return false;

    // OpenGL state
    glEnable(GL_DEPTH_TEST);

    // Geometry
    m_mesh.create(
        m_pyramid.get_vertices(),
        m_pyramid.get_indices()
    );

    // Shader
    if (!create_shader())
    {
        shutdown();
        return false;
    }

    // Uniforms
    m_mvp_location = m_shader.get_uniform_location("u_mvp");

    if (m_mvp_location == -1)
    {
        spdlog::error("Failed to find uniform location for u_mvp");
        shutdown();
        return false;
    }

    m_is_initialized = true;

    return true;
}

void Renderer::shutdown()
{
    m_mesh.destroy();
    m_shader.destroy();

    m_mvp_location = -1;
    m_is_initialized = false;
}

void Renderer::set_viewport(int width, int height)
{
    glViewport(0, 0, width, height);
}

void Renderer::render(
    const OrbitCamera& camera,
    int width,
    int height
)
{
    if (!m_is_initialized || width <= 0 || height <= 0)
        return;

    // Clear
    glClearColor(
        BACKGROUND_COLOR[0],
        BACKGROUND_COLOR[1],
        BACKGROUND_COLOR[2],
        BACKGROUND_COLOR[3]
    );

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Projection
    const glm::mat4 projection = create_projection_matrix(width, height);

    // View
    const glm::mat4 view = camera.get_view_matrix();

    // Model
    const glm::mat4 model = glm::scale(
        glm::mat4(1.0f),
        glm::vec3(1.4f, 1.4f, 1.4f)
    );

    // MVP
    const glm::mat4 mvp = projection * view * model;

    // Shader
    m_shader.use();
    glUniformMatrix4fv(m_mvp_location, 1, GL_FALSE, glm::value_ptr(mvp));

    // Geometry
    m_mesh.draw();
}

bool Renderer::create_shader()
{
    const std::filesystem::path vertex_shader_path = "../res/shader_program_sources/vertex.glsl";
    const std::filesystem::path fragment_shader_path = "../res/shader_program_sources/fragment.glsl";

    std::string vertex_shader_source;
    std::string fragment_shader_source;

    try
    {
        vertex_shader_source = andromeda::utils::FileOperations::load_file_as_string(vertex_shader_path);
        fragment_shader_source = andromeda::utils::FileOperations::load_file_as_string(fragment_shader_path);
    }
    catch (const std::exception& exception)
    {
        spdlog::error("Failed to load shader files: {}", exception.what());
        return false;
    }

    return m_shader.create(
        vertex_shader_source,
        fragment_shader_source
    );
}

glm::mat4 Renderer::create_projection_matrix(
    int width,
    int height
) const
{
    const float aspect_ratio =
        static_cast<float>(width) /
        static_cast<float>(height);

    return glm::perspective(
        glm::radians(FOV_DEGREES_Y_AXIS),
        aspect_ratio,
        NEAR_PLANE,
        FAR_PLANE
    );
}