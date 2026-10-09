#pragma once


#include "vertex.hpp"

#include "glad/gl.h"

#include <vector>


class Mesh
{
public:
    Mesh();
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    void create(
        const std::vector<Vertex>& vertices,
        const std::vector<unsigned int>& indices
    );

    void destroy();
    void draw() const;

private:
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_ebo;

    int m_index_count;
};