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

    void on_mouse_button(
        int button,
        int action
    );

    void on_mouse_move(
        double x,
        double y,
        bool is_ctrl_down
    );

    glm::mat4 get_view_matrix() const;


private:
    void orbit(
        float yaw_delta,
        float pitch_delta
    );

    void roll(
        float roll_delta
    );


private:
    glm::vec3 m_base_position;

    glm::quat m_orbit_orientation;
    glm::quat m_roll_orientation;

    bool m_is_left_mouse_down = false;
    bool m_has_last_mouse_position = false;

    double m_last_mouse_x = 0.0;
    double m_last_mouse_y = 0.0;
};