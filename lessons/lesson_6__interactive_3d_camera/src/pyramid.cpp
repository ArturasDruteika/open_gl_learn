#include "pyramid.hpp"

#include "glm/glm.hpp"

#include <cstddef>
#include <vector>


namespace
{
    struct Vertex
    {
        glm::vec3 position;
        glm::vec4 color;
    };
}


Pyramid::~Pyramid()
{
    destroy();
}


void Pyramid::create()
{
    destroy();


    // -------------------------------------------------------------------------
    // Positions
    // -------------------------------------------------------------------------

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


    // -------------------------------------------------------------------------
    // Colors
    // -------------------------------------------------------------------------

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


    // -------------------------------------------------------------------------
    // Vertices
    // -------------------------------------------------------------------------

    const std::vector<Vertex> vertices =
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


    // -------------------------------------------------------------------------
    // Indices
    // -------------------------------------------------------------------------

    const std::vector<unsigned int> indices =
    {
        0, 1, 2,

        3, 4, 5,

        6, 7, 8,

        9, 10, 11,

        12, 13, 14,

        12, 14, 15
    };


    m_index_count =
        static_cast<GLsizei>(
            indices.size()
        );


    // -------------------------------------------------------------------------
    // OpenGL objects
    // -------------------------------------------------------------------------

    glGenVertexArrays(
        1,
        &m_vao
    );


    glGenBuffers(
        1,
        &m_vbo
    );


    glGenBuffers(
        1,
        &m_ebo
    );


    glBindVertexArray(
        m_vao
    );


    // -------------------------------------------------------------------------
    // VBO
    // -------------------------------------------------------------------------

    glBindBuffer(
        GL_ARRAY_BUFFER,
        m_vbo
    );


    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(
            vertices.size() *
            sizeof(Vertex)
        ),
        vertices.data(),
        GL_STATIC_DRAW
    );


    // -------------------------------------------------------------------------
    // EBO
    // -------------------------------------------------------------------------

    glBindBuffer(
        GL_ELEMENT_ARRAY_BUFFER,
        m_ebo
    );


    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(
            indices.size() *
            sizeof(unsigned int)
        ),
        indices.data(),
        GL_STATIC_DRAW
    );


    // -------------------------------------------------------------------------
    // Position attribute
    // -------------------------------------------------------------------------

    glVertexAttribPointer(
        0,
        3,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(
            offsetof(
                Vertex,
                position
            )
        )
    );


    glEnableVertexAttribArray(
        0
    );


    // -------------------------------------------------------------------------
    // Color attribute
    // -------------------------------------------------------------------------

    glVertexAttribPointer(
        1,
        4,
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        reinterpret_cast<void*>(
            offsetof(
                Vertex,
                color
            )
        )
    );


    glEnableVertexAttribArray(
        1
    );


    glBindVertexArray(
        0
    );
}


void Pyramid::destroy()
{
    if (m_ebo != 0)
    {
        glDeleteBuffers(
            1,
            &m_ebo
        );

        m_ebo = 0;
    }


    if (m_vbo != 0)
    {
        glDeleteBuffers(
            1,
            &m_vbo
        );

        m_vbo = 0;
    }


    if (m_vao != 0)
    {
        glDeleteVertexArrays(
            1,
            &m_vao
        );

        m_vao = 0;
    }


    m_index_count = 0;
}


void Pyramid::draw() const
{
    glBindVertexArray(
        m_vao
    );


    glDrawElements(
        GL_TRIANGLES,
        m_index_count,
        GL_UNSIGNED_INT,
        nullptr
    );
}