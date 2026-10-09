#pragma once


#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"


class OrbitCamera
{
public:
    OrbitCamera(
        const glm::vec3& base_position,
        float initial_pitch,
        float initial_yaw,
        float initial_roll
    );

    void orbit(
        float yaw_delta,
        float pitch_delta
    );

    void roll(float roll_delta);
    void zoom(float distance_delta);

    glm::mat4 get_view_matrix() const;

private:
    glm::vec3 m_base_position;
    glm::quat m_orbit_orientation;

    float m_distance;
};