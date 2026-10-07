#pragma once

#include "glad/gl.h"


class Pyramid
{
public:
    Pyramid();
    ~Pyramid();
    
    Pyramid(const Pyramid&) = delete;
    Pyramid& operator=(const Pyramid&) = delete;

    void create();
    void destroy();
    void draw() const;

private:
    unsigned int m_vao;
    unsigned int m_vbo;
    unsigned int m_ebo;

    GLsizei m_index_count;
};