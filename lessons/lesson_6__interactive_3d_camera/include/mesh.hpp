#pragma once


#include "vertex.hpp"

#include <cstddef>
#include <vector>


class Mesh {
public:
    Mesh(std::vector<Vertex> vertices, 
         std::vector<unsigned int> indices);
         
    ~Mesh();

    const std::vector<Vertex>& vertices() const noexcept;
    const std::vector<unsigned int>& indices() const noexcept;
    std::size_t triangle_count() const noexcept;

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
};