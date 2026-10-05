#pragma once

#include "glad/gl.h"


class Pyramid
{
public:
    Pyramid() = default;

    ~Pyramid();


    Pyramid(
        const Pyramid&
    ) = delete;


    Pyramid& operator=(
        const Pyramid&
    ) = delete;


    void create();

    void destroy();

    void draw() const;


private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;

    GLsizei m_index_count = 0;
};