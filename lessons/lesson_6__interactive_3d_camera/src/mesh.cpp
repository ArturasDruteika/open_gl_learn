#include "mesh.hpp"

#include <cstddef>


Mesh::Mesh()
    : m_vao{ 0 }
    , m_vbo{ 0 }
    , m_ebo{ 0 }
    , m_index_count{ 0 }
{
}

Mesh::~Mesh()
{
    destroy();
}

void Mesh::create(
    const std::vector<Vertex>& vertices,
    const std::vector<unsigned int>& indices
)
{
    destroy();

    m_index_count = static_cast<GLsizei>(indices.size());

    // OpenGL objects
    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);

    glBindVertexArray(m_vao);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);

    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(vertices.size() * sizeof(Vertex)),
        vertices.data(),
        GL_STATIC_DRAW
    );

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(indices.size() * sizeof(unsigned int)),
        indices.data(),
        GL_STATIC_DRAW
    );

    // Position attribute
    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, position))
    );

    glEnableVertexAttribArray(0);

    // Color attribute
    glVertexAttribPointer(
        1,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(offsetof(Vertex, color))
    );

    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Mesh::destroy()
{
    if (m_ebo != 0)
    {
        glDeleteBuffers(1, &m_ebo);
        m_ebo = 0;
    }

    if (m_vbo != 0)
    {
        glDeleteBuffers(1, &m_vbo);
        m_vbo = 0;
    }

    if (m_vao != 0)
    {
        glDeleteVertexArrays(1, &m_vao);
        m_vao = 0;
    }

    m_index_count = 0;
}

void Mesh::draw() const
{
    glBindVertexArray(m_vao);

    glDrawElements(
        GL_TRIANGLES,
        m_index_count,
        GL_UNSIGNED_INT,
        nullptr
    );
}