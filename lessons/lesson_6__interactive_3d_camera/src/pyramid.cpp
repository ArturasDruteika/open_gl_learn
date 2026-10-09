#include "pyramid.hpp"


Pyramid::Pyramid()
{
    // Positions
    const glm::vec3 top =
    {
        0.0f,
        0.35f,
        0.0f
    };

    const glm::vec3 front_left =
    {
        -0.25f,
        -0.25f,
        0.25f
    };

    const glm::vec3 front_right =
    {
        0.25f,
        -0.25f,
        0.25f
    };

    const glm::vec3 back_left =
    {
        -0.25f,
        -0.25f,
        -0.25f
    };

    const glm::vec3 back_right =
    {
        0.25f,
        -0.25f,
        -0.25f
    };

    // Colors
    const glm::vec4 neon_blue_1 =
    {
        0.0f,
        0.3f,
        0.8f,
        1.0f
    };

    const glm::vec4 neon_blue_2 =
    {
        0.0f,
        0.7f,
        1.0f,
        1.0f
    };

    const glm::vec4 neon_green =
    {
        0.2f,
        1.0f,
        0.2f,
        1.0f
    };

    const glm::vec4 neon_purple =
    {
        0.8f,
        0.0f,
        1.0f,
        1.0f
    };

    const glm::vec4 base_blue =
    {
        0.0f,
        0.4f,
        0.8f,
        1.0f
    };

    const glm::vec4 base_purple =
    {
        0.4f,
        0.0f,
        0.8f,
        1.0f
    };

    // Vertices
    m_vertices =
    {
        // Front
        { top,         neon_blue_1 },
        { front_left,  neon_blue_2 },
        { front_right, neon_blue_2 },

        // Right
        { top,         neon_blue_1 },
        { front_right, neon_green },
        { back_right,  neon_blue_2 },

        // Back
        { top,        neon_purple },
        { back_right, neon_purple },
        { back_left,  neon_purple },

        // Left
        { top,        neon_blue_2 },
        { back_left,  neon_blue_1 },
        { front_left, neon_blue_1 },

        // Base
        { front_left,  base_blue },
        { back_left,   base_blue },
        { back_right,  base_purple },
        { front_right, base_purple }
    };

    // Indices
    m_indices =
    {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        12, 14, 15
    };
}

Pyramid::~Pyramid() = default;

const std::vector<Vertex>& Pyramid::get_vertices() const
{
    return m_vertices;
}

const std::vector<unsigned int>& Pyramid::get_indices() const
{
    return m_indices;
}