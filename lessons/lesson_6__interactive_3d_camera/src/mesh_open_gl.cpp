#include "mesh_open_gl.hpp"

#include <cstddef>
#include <limits>
#include <stdexcept>


MeshOpenGL::MeshOpenGL(const Mesh& mesh) 
{
    if (mesh.indices().size() > static_cast<std::size_t>(std::numeric_limits<GLsizei>::max()))
        throw std::length_error("Too many indices for OpenGL");

    m_index_count = static_cast<GLsizei>(mesh.indices().size());

    glGenVertexArrays(1, &m_vao);
    glGenBuffers(1, &m_vbo);
    glGenBuffers(1, &m_ebo);
    glBindVertexArray(m_vao);

    glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
    glBufferData(GL_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(mesh.vertices().size() * sizeof(Vertex)),
                 mesh.vertices().data(), 
                 GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 static_cast<GLsizeiptr>(mesh.indices().size() * sizeof(unsigned int)),
                 mesh.indices().data(), 
                 GL_STATIC_DRAW);

    glVertexAttribPointer(0, 
                          3, 
                          GL_FLOAT, 
                          GL_FALSE, 
                          sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, 
                                                           position)));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 
                          4, 
                          GL_FLOAT, 
                          GL_FALSE, 
                          sizeof(Vertex),
                          reinterpret_cast<void*>(offsetof(Vertex, 
                                                           color)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

MeshOpenGL::~MeshOpenGL() 
{
    if (m_ebo) 
        glDeleteBuffers(1, &m_ebo);

    if (m_vbo) 
        glDeleteBuffers(1, &m_vbo);

    if (m_vao) 
        glDeleteVertexArrays(1, &m_vao);
}

void MeshOpenGL::draw() const 
{
    glBindVertexArray(m_vao);
    glDrawElements(GL_TRIANGLES, 
                   m_index_count, 
                   GL_UNSIGNED_INT, 
                   nullptr);
    glBindVertexArray(0);
}
