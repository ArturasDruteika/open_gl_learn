#pragma once


#include "mesh.hpp"

#include <glm/glm.hpp>


class Shape
{
public:
    virtual ~Shape();

    const Mesh& get_mesh() const;

    const glm::vec3& get_position() const;
    void set_position(const glm::vec3& position);

    const glm::vec3& get_rotation() const;
    void set_rotation(const glm::vec3& rotation);

    const glm::vec3& get_scale() const;
    void set_scale(const glm::vec3& scale);

    glm::mat4 get_model_matrix() const;

protected:
    explicit Shape(const glm::vec3& position, Mesh mesh);

private:
    Mesh m_mesh;

    glm::vec3 m_position{ 0.0f };
    glm::vec3 m_rotation{ 0.0f };
    glm::vec3 m_scale{ 1.0f };
};