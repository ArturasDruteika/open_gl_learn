#include "camera_orbit_controller.hpp"

#include "GLFW/glfw3.h"


namespace
{
    constexpr float MOUSE_ORBIT_X_SENSITIVITY = 0.005f;
    constexpr float MOUSE_ORBIT_Y_SENSITIVITY = 0.005f;
    constexpr float MOUSE_ROLL_SENSITIVITY = 0.005f;

    constexpr float MOUSE_ZOOM_SENSITIVITY = 0.2f;
}


OrbitCameraController::OrbitCameraController(OrbitCamera& camera)
    : m_camera{ camera }
    , m_is_left_mouse_down{ false }
    , m_has_last_mouse_position{ false }
    , m_last_mouse_x{ 0.0 }
    , m_last_mouse_y{ 0.0 }
{
}

void OrbitCameraController::on_mouse_button(
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

void OrbitCameraController::on_cursor_position(
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
        m_camera.roll(dx * MOUSE_ROLL_SENSITIVITY);
        return;
    }

    // Mouse X -> yaw
    // Mouse Y -> pitch
    m_camera.orbit(
        dx * MOUSE_ORBIT_Y_SENSITIVITY,
        dy * MOUSE_ORBIT_X_SENSITIVITY
    );
}

void OrbitCameraController::on_mouse_scroll(double y_offset)
{
    m_camera.zoom(
        -static_cast<float>(y_offset) * MOUSE_ZOOM_SENSITIVITY
    );
}