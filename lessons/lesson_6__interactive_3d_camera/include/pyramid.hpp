#pragma once


#include "shape.hpp"


class Pyramid final 
    : public Shape
{
public:
    Pyramid(float width = 0.5f,
            float depth = 0.5f,
            float height = 0.6f);

    ~Pyramid();

private:
    static Mesh create_mesh(float width,
                            float depth,
                            float height);

private:
    float m_width;
    float m_depth;
    float m_height;
};