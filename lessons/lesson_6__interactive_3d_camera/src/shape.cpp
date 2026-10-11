#include "shape.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <utility>


Shape::Shape(Mesh mesh)
    : m_mesh{ std::move(mesh) }
{
}

Shape::~Shape() = default;

const Mesh& Shape::get_mesh() const
{
    return m_mesh;
}

const glm::vec3& Shape::get_position() const
{
    return m_position;
}

void Shape::set_position(const glm::vec3& position)
{
    m_position = position;
}

const glm::vec3& Shape::get_rotation() const
{
    return m_rotation;
}

void Shape::set_rotation(const glm::vec3& rotation)
{
    m_rotation = rotation;
}

const glm::vec3& Shape::get_scale() const
{
    return m_scale;
}

void Shape::set_scale(const glm::vec3& scale)
{
    m_scale = scale;
}

glm::mat4 Shape::get_model_matrix() const
{
    glm::mat4 model{ 1.0f };

    model = glm::translate(model, m_position);

    model = glm::rotate(model,
                        glm::radians(m_rotation.x),
                        glm::vec3{ 1.0f, 0.0f, 0.0f });

    model = glm::rotate(model,
                        glm::radians(m_rotation.y),
                        glm::vec3{ 0.0f, 1.0f, 0.0f });

    model = glm::rotate(model,
                        glm::radians(m_rotation.z),
                        glm::vec3{ 0.0f, 0.0f, 1.0f });

    model = glm::scale(model, m_scale);

    return model;
}