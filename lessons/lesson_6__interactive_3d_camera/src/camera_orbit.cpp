#include "camera_orbit.hpp"

#include "GLFW/glfw3.h"

#include "glm/gtc/matrix_transform.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/gtx/quaternion.hpp"


namespace
{
    constexpr float MOUSE_ORBIT_X_SENSITIVITY = 0.005f;
    constexpr float MOUSE_ORBIT_Y_SENSITIVITY = 0.005f;
    constexpr float MOUSE_ROLL_SENSITIVITY = 0.005f;
}


OrbitCamera::OrbitCamera(
    const glm::vec3& base_position,
    float initial_pitch,
    float initial_yaw,
    float initial_roll
)
    : m_is_left_mouse_down{ false }
    , m_has_last_mouse_position{ false }
    , m_last_mouse_x{ 0.0 }
    , m_last_mouse_y{ 0.0 }
    , m_base_position{ base_position }
{
    m_orbit_orientation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);

    // Build the initial orientation using the same semantics as normal camera
    // rotation: yaw, then pitch, then roll.
    orbit(initial_yaw, initial_pitch);
    roll(initial_roll);
}


void OrbitCamera::on_mouse_button(
    int button,
    int action
)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT)
        return;


    if (action == GLFW_PRESS)
    {
        m_is_left_mouse_down = true;
        m_has_last_mouse_position = false;
    }
    else if (action == GLFW_RELEASE)
    {
        m_is_left_mouse_down = false;
        m_has_last_mouse_position = false;
    }
}


void OrbitCamera::on_mouse_move(
    double x,
    double y,
    bool is_ctrl_down
)
{
    if (!m_is_left_mouse_down)
    {
        m_last_mouse_x = x;
        m_last_mouse_y = y;

        m_has_last_mouse_position = true;

        return;
    }

    if (!m_has_last_mouse_position)
    {
        m_last_mouse_x = x;
        m_last_mouse_y = y;

        m_has_last_mouse_position = true;

        return;
    }

    const float dx = static_cast<float>(x - m_last_mouse_x);
    const float dy = static_cast<float>(y - m_last_mouse_y);

    m_last_mouse_x = x;
    m_last_mouse_y = y;

    // CTRL + left mouse -> roll
    if (is_ctrl_down)
    {
        roll(dx * MOUSE_ROLL_SENSITIVITY);

        return;
    }


    // Same behavior as Andromeda:
    //
    // mouse X -> yaw
    // mouse Y -> pitch
    //
    // Do NOT undo roll here. Yaw/pitch work in the camera's current local
    // coordinate system, so roll naturally changes the axes they operate on.
    orbit(
        dx * MOUSE_ORBIT_Y_SENSITIVITY,
        dy * MOUSE_ORBIT_X_SENSITIVITY
    );
}


void OrbitCamera::orbit(
    float yaw_delta,
    float pitch_delta
)
{
    // -------------------------------------------------------------------------
    // Yaw
    //
    // Rotate around the camera's CURRENT local up axis.
    // -------------------------------------------------------------------------
    if (yaw_delta != 0.0f)
    {
        const glm::vec3 up_axis = glm::normalize(m_orbit_orientation * glm::vec3(0.0f, 1.0f, 0.0f));
        const glm::quat yaw_rotation = glm::angleAxis(-yaw_delta, up_axis);
        m_orbit_orientation = glm::normalize(yaw_rotation * m_orbit_orientation);
    }


    // -------------------------------------------------------------------------
    // Pitch
    //
    // Recalculate the local right axis AFTER yaw.
    // -------------------------------------------------------------------------
    if (pitch_delta != 0.0f)
    {
        const glm::vec3 right_axis = glm::normalize(m_orbit_orientation * glm::vec3(1.0f, 0.0f, 0.0f));
        const glm::quat pitch_rotation = glm::angleAxis(-pitch_delta, right_axis);
        m_orbit_orientation = glm::normalize(pitch_rotation * m_orbit_orientation);
    }
}


void OrbitCamera::roll(float roll_delta)
{
    if (roll_delta == 0.0f)
        return;

    // Roll around the camera's CURRENT forward axis.
    const glm::vec3 forward_axis = glm::normalize(m_orbit_orientation * glm::vec3(0.0f, 0.0f, -1.0f));
    const glm::quat roll_rotation = glm::angleAxis(roll_delta, forward_axis);
    m_orbit_orientation = glm::normalize(roll_rotation * m_orbit_orientation);
}


glm::mat4 OrbitCamera::get_view_matrix() const
{
    const glm::quat camera_orientation = glm::normalize(m_orbit_orientation);

    // The camera orbits around the origin.
    //
    // The initial base position determines the orbit radius/direction,
    // and the complete camera orientation rotates that position.
    const glm::vec3 camera_position = camera_orientation * m_base_position;

    // Derive the camera's actual up vector from its orientation.
    //
    // This preserves roll, unlike glm::lookAt() with a constant world-up.
    const glm::vec3 camera_up = glm::normalize(camera_orientation * glm::vec3(0.0f, 1.0f, 0.0f));

    return glm::lookAt(
        camera_position,
        glm::vec3(0.0f, 0.0f, 0.0f),
        camera_up
    );
}