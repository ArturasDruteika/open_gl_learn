#pragma once


#include "mesh.hpp"
#include "glad/gl.h"


class MeshOpenGL {
public:
    explicit MeshOpenGL(const Mesh& mesh);
    ~MeshOpenGL();

    MeshOpenGL(const MeshOpenGL&) = delete;
    MeshOpenGL& operator=(const MeshOpenGL&) = delete;
    MeshOpenGL(MeshOpenGL&&) = delete;
    MeshOpenGL& operator=(MeshOpenGL&&) = delete;

    void draw() const;

private:
    unsigned int m_vao = 0;
    unsigned int m_vbo = 0;
    unsigned int m_ebo = 0;
    int m_index_count = 0;
};

