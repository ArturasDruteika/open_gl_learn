#include "mesh.hpp"

#include <stdexcept>
#include <utility>


Mesh::Mesh(std::vector<Vertex> vertices,
           std::vector<unsigned int> indices)
    : m_vertices{ std::move(vertices) }
    , m_indices{ std::move(indices) }
{
    if (m_indices.size() % 3 != 0)
        throw std::invalid_argument("Mesh indices must describe triangles");

    for (unsigned int index : m_indices)
    {
        if (index >= m_vertices.size())
            throw std::out_of_range("Mesh index is outside vertex array");
    }
}

Mesh::~Mesh() = default;

const std::vector<Vertex>& Mesh::vertices() const noexcept
{
    return m_vertices;
}

const std::vector<unsigned int>& Mesh::indices() const noexcept
{
    return m_indices;
}

std::size_t Mesh::triangle_count() const noexcept
{
    return m_indices.size() / 3;
}