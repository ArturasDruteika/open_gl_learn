#include "camera_orbit.hpp"

#include "glm/gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"

#include <algorithm>


namespace
{
    constexpr float MIN_CAMERA_DISTANCE = 0.5f;
    constexpr float MAX_CAMERA_DISTANCE = 10.0f;
}


OrbitCamera::OrbitCamera(
    const glm::vec3& base_position,
    float initial_pitch,
    float initial_yaw,
    float initial_roll
)
    : m_base_position{ glm::normalize(base_position) }
    , m_orbit_orientation{ 1.0f, 0.0f, 0.0f, 0.0f }
    , m_distance{ glm::length(base_position) }
{
    orbit(initial_yaw, initial_pitch);
    roll(initial_roll);
}

void OrbitCamera::orbit(
    float yaw_delta,
    float pitch_delta
)
{
    // Yaw
    if (yaw_delta != 0.0f)
    {
        const glm::vec3 up_axis = glm::normalize(
            m_orbit_orientation * glm::vec3(0.0f, 1.0f, 0.0f)
        );

        const glm::quat yaw_rotation = glm::angleAxis(
            -yaw_delta,
            up_axis
        );

        m_orbit_orientation = glm::normalize(
            yaw_rotation * m_orbit_orientation
        );
    }

    // Pitch
    if (pitch_delta != 0.0f)
    {
        const glm::vec3 right_axis = glm::normalize(
            m_orbit_orientation * glm::vec3(1.0f, 0.0f, 0.0f)
        );

        const glm::quat pitch_rotation = glm::angleAxis(
            -pitch_delta,
            right_axis
        );

        m_orbit_orientation = glm::normalize(
            pitch_rotation * m_orbit_orientation
        );
    }
}

void OrbitCamera::roll(float roll_delta)
{
    if (roll_delta == 0.0f)
        return;

    const glm::vec3 forward_axis = glm::normalize(
        m_orbit_orientation * glm::vec3(0.0f, 0.0f, -1.0f)
    );

    const glm::quat roll_rotation = glm::angleAxis(
        roll_delta,
        forward_axis
    );

    m_orbit_orientation = glm::normalize(
        roll_rotation * m_orbit_orientation
    );
}

void OrbitCamera::zoom(float distance_delta)
{
    m_distance = std::clamp(
        m_distance + distance_delta,
        MIN_CAMERA_DISTANCE,
        MAX_CAMERA_DISTANCE
    );
}

glm::mat4 OrbitCamera::get_view_matrix() const
{
    const glm::quat camera_orientation = glm::normalize(
        m_orbit_orientation
    );

    const glm::vec3 camera_position =
        camera_orientation * (m_base_position * m_distance);

    const glm::vec3 camera_up = glm::normalize(
        camera_orientation * glm::vec3(0.0f, 1.0f, 0.0f)
    );

    return glm::lookAt(
        camera_position,
        glm::vec3(0.0f, 0.0f, 0.0f),
        camera_up
    );
}