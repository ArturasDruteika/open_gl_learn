#pragma once


#include "shape.hpp"


class Pyramid final 
    : public Shape
{
public:
    Pyramid(float width = 0.5f,
            float length = 0.5f,
            float height = 0.6f);

    ~Pyramid();

private:
    static Mesh create_mesh(float width,
                            float length,
                            float height);

private:
    float m_width;
    float m_length;
    float m_height;
};