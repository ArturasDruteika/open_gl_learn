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
    :
    m_base_position(base_position)
{
    const glm::quat pitch_orientation =
        glm::angleAxis(
            initial_pitch,
            glm::vec3(1.0f, 0.0f, 0.0f)
        );


    const glm::quat yaw_orientation =
        glm::angleAxis(
            initial_yaw,
            glm::vec3(0.0f, 1.0f, 0.0f)
        );


    m_orbit_orientation =
        glm::normalize(
            yaw_orientation *
            pitch_orientation
        );


    m_roll_orientation =
        glm::angleAxis(
            initial_roll,
            glm::vec3(0.0f, 0.0f, 1.0f)
        );
}


void OrbitCamera::on_mouse_button(
    int button,
    int action
)
{
    if (button != GLFW_MOUSE_BUTTON_LEFT)
    {
        return;
    }


    if (action == GLFW_PRESS)
    {
        m_is_left_mouse_down = true;

        // Prevent the first mouse movement from causing a jump.
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
    // -------------------------------------------------------------------------
    // Mouse is not being dragged
    // -------------------------------------------------------------------------

    if (!m_is_left_mouse_down)
    {
        m_last_mouse_x = x;
        m_last_mouse_y = y;

        m_has_last_mouse_position = true;

        return;
    }


    // -------------------------------------------------------------------------
    // First mouse event after pressing the left mouse button
    // -------------------------------------------------------------------------

    if (!m_has_last_mouse_position)
    {
        m_last_mouse_x = x;
        m_last_mouse_y = y;

        m_has_last_mouse_position = true;

        return;
    }


    // -------------------------------------------------------------------------
    // Mouse movement
    // -------------------------------------------------------------------------

    const float dx =
        static_cast<float>(
            x - m_last_mouse_x
        );


    const float dy =
        static_cast<float>(
            y - m_last_mouse_y
        );


    m_last_mouse_x = x;
    m_last_mouse_y = y;


    // -------------------------------------------------------------------------
    // CTRL + left mouse -> roll
    // -------------------------------------------------------------------------

    if (is_ctrl_down)
    {
        roll(
            dx *
            MOUSE_ROLL_SENSITIVITY
        );

        return;
    }


    // -------------------------------------------------------------------------
    // Left mouse -> orbit
    // -------------------------------------------------------------------------

    // Horizontal direction is intentionally inverted.
    const float yaw_delta =
        -dx *
        MOUSE_ORBIT_Y_SENSITIVITY;


    const float pitch_delta =
        dy *
        MOUSE_ORBIT_X_SENSITIVITY;


    orbit(
        yaw_delta,
        pitch_delta
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
    // Rotate around world Y.
    // -------------------------------------------------------------------------

    const glm::quat yaw_rotation =
        glm::angleAxis(
            yaw_delta,
            glm::vec3(
                0.0f,
                1.0f,
                0.0f
            )
        );


    m_orbit_orientation =
        glm::normalize(
            yaw_rotation *
            m_orbit_orientation
        );


    // -------------------------------------------------------------------------
    // Pitch
    //
    // Rotate around the camera's current orbital X axis.
    // -------------------------------------------------------------------------

    const glm::vec3 pitch_axis =
        glm::normalize(
            m_orbit_orientation *
            glm::vec3(
                1.0f,
                0.0f,
                0.0f
            )
        );


    const glm::quat pitch_rotation =
        glm::angleAxis(
            pitch_delta,
            pitch_axis
        );


    m_orbit_orientation =
        glm::normalize(
            pitch_rotation *
            m_orbit_orientation
        );
}


void OrbitCamera::roll(
    float roll_delta
)
{
    const glm::quat roll_rotation =
        glm::angleAxis(
            roll_delta,
            glm::vec3(
                0.0f,
                0.0f,
                1.0f
            )
        );


    // Roll is local to the camera.
    m_roll_orientation =
        glm::normalize(
            m_roll_orientation *
            roll_rotation
        );
}


glm::mat4 OrbitCamera::get_view_matrix() const
{
    const glm::quat orbit_orientation =
        glm::normalize(
            m_orbit_orientation
        );


    const glm::quat roll_orientation =
        glm::normalize(
            m_roll_orientation
        );


    // -------------------------------------------------------------------------
    // Camera position
    //
    // Rotating the base position moves the camera around the origin while
    // preserving its distance from the origin.
    // -------------------------------------------------------------------------

    const glm::vec3 camera_position =
        orbit_orientation *
        m_base_position;


    // -------------------------------------------------------------------------
    // Camera orientation
    //
    // Roll affects camera orientation without affecting orbital position.
    // -------------------------------------------------------------------------

    const glm::quat camera_orientation =
        glm::normalize(
            orbit_orientation *
            roll_orientation
        );


    // -------------------------------------------------------------------------
    // View rotation
    //
    // The view matrix is the inverse camera transformation.
    // For a normalized quaternion, inverse == conjugate.
    // -------------------------------------------------------------------------

    const glm::quat inverse_camera_orientation =
        glm::conjugate(
            camera_orientation
        );


    const glm::mat4 view_rotation =
        glm::toMat4(
            inverse_camera_orientation
        );


    // -------------------------------------------------------------------------
    // View translation
    // -------------------------------------------------------------------------

    const glm::mat4 view_translation =
        glm::translate(
            glm::mat4(1.0f),
            -camera_position
        );


    return
        view_rotation *
        view_translation;
}