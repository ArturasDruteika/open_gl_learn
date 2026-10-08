#pragma once

#include "camera_orbit.hpp"


class OrbitCameraController
{
public:
    explicit OrbitCameraController(OrbitCamera& camera);

    void on_mouse_button(
        int button,
        int action
    );

    void on_cursor_position(
        double x,
        double y,
        bool is_ctrl_down
    );

    void on_mouse_scroll(double y_offset);

private:
    bool m_is_left_mouse_down;
    bool m_has_last_mouse_position;
    double m_last_mouse_x;
    double m_last_mouse_y;

    OrbitCamera& m_camera;
};