#include "pyramid.hpp"

#include <stdexcept>
#include <utility>
#include <vector>


Pyramid::Pyramid(float width, float depth, float height)
    : Shape{ create_mesh(width, depth, height) }
    , m_width{ width }
    , m_depth{ depth }
    , m_height{ height }
{
}

Pyramid::~Pyramid() = default;


Mesh Pyramid::create_mesh(float width,
                          float depth,
                          float height)
{
    if (!(width > 0.0f && depth > 0.0f && height > 0.0f))
        throw std::invalid_argument("Pyramid dimensions must be positive");

    const float x = width / 2.0f;
    const float z = depth / 2.0f;

    const float bottom = -height / 2.0f;
    const float top_y = height / 2.0f;

    // Positions
    const glm::vec3 top{ 0.0f, top_y, 0.0f };

    const glm::vec3 front_left{ -x, bottom, z };
    const glm::vec3 front_right{ x, bottom, z };

    const glm::vec3 back_left{ -x, bottom, -z };
    const glm::vec3 back_right{ x, bottom, -z };

    // Colors
    const glm::vec4 neon_blue_1{ 0.0f, 0.3f, 0.8f, 1.0f };
    const glm::vec4 neon_blue_2{ 0.0f, 0.7f, 1.0f, 1.0f };

    const glm::vec4 neon_green{ 0.2f, 1.0f, 0.2f, 1.0f };
    const glm::vec4 neon_purple{ 0.8f, 0.0f, 1.0f, 1.0f };

    const glm::vec4 base_blue{ 0.0f, 0.4f, 0.8f, 1.0f };
    const glm::vec4 base_purple{ 0.4f, 0.0f, 0.8f, 1.0f };

    // Vertices
    std::vector<Vertex> vertices
    {
        // Front
        { top, neon_blue_1 },
        { front_left, neon_blue_2 },
        { front_right, neon_blue_2 },

        // Right
        { top, neon_blue_1 },
        { front_right, neon_green },
        { back_right, neon_blue_2 },

        // Back
        { top, neon_purple },
        { back_right, neon_purple },
        { back_left, neon_purple },

        // Left
        { top, neon_blue_2 },
        { back_left, neon_blue_1 },
        { front_left, neon_blue_1 },

        // Base
        { front_left, base_blue },
        { back_left, base_blue },
        { back_right, base_purple },
        { front_right, base_purple }
    };

    // Indices
    std::vector<unsigned int> indices
    {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        12, 14, 15
    };

    return Mesh{
        std::move(vertices),
        std::move(indices)
    };
}