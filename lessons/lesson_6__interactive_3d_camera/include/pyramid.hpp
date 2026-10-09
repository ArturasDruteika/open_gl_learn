#pragma once


#include "vertex.hpp"

#include <vector>


class Pyramid
{
public:
    Pyramid();
    ~Pyramid();

    const std::vector<Vertex>& get_vertices() const;
    const std::vector<unsigned int>& get_indices() const;

private:
    std::vector<Vertex> m_vertices;
    std::vector<unsigned int> m_indices;
};