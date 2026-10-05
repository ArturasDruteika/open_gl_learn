#include "camera_orbit.hpp"
#include "pyramid.hpp"
#include "shader_program.hpp"

#include "andromeda/utils/file_operations/file_operations.hpp"

#include "glad/gl.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "spdlog/spdlog.h"

#include <exception>
#include <filesystem>
#include <string>


// =============================================================================
// Constants
// =============================================================================

constexpr int OPENGL_MAJOR_VERSION = 4;
constexpr int OPENGL_MINOR_VERSION = 6;

constexpr int WINDOW_WIDTH = 800;
constexpr int WINDOW_HEIGHT = 800;

constexpr float BACKGROUND_COLOR[4] =
{
    0.1f,
    0.2f,
    0.3f,
    1.0f
};

constexpr float FOV_DEGREES_Y_AXIS = 45.0f;
constexpr float NEAR_PLANE = 0.1f;
constexpr float FAR_PLANE = 100.0f;


// =============================================================================
// Application
// =============================================================================

class Application
{
public:
    Application()
        :
        m_camera(
            glm::vec3(
                0.0f,
                0.0f,
                2.5f
            ),
            glm::radians(20.0f),
            glm::radians(-35.0f),
            glm::radians(90.0f)
        )
    {
    }


    ~Application()
    {
        shutdown();
    }


    Application(
        const Application&
    ) = delete;


    Application& operator=(
        const Application&
    ) = delete;


    int run()
    {
        if (!initialize())
        {
            shutdown();

            return -1;
        }


        render_loop();


        shutdown();

        return 0;
    }


private:

    // =========================================================================
    // Initialization
    // =========================================================================

    bool initialize()
    {
        // ---------------------------------------------------------------------
        // GLFW
        // ---------------------------------------------------------------------

        if (!glfwInit())
        {
            spdlog::error(
                "Failed to initialize GLFW"
            );

            return false;
        }


        m_is_glfw_initialized = true;


        glfwWindowHint(
            GLFW_CONTEXT_VERSION_MAJOR,
            OPENGL_MAJOR_VERSION
        );


        glfwWindowHint(
            GLFW_CONTEXT_VERSION_MINOR,
            OPENGL_MINOR_VERSION
        );


        glfwWindowHint(
            GLFW_OPENGL_PROFILE,
            GLFW_OPENGL_CORE_PROFILE
        );


        // ---------------------------------------------------------------------
        // Window
        // ---------------------------------------------------------------------

        m_window =
            glfwCreateWindow(
                WINDOW_WIDTH,
                WINDOW_HEIGHT,
                "3D Pyramid",
                nullptr,
                nullptr
            );


        if (!m_window)
        {
            spdlog::error(
                "Failed to create GLFW window"
            );

            return false;
        }


        glfwMakeContextCurrent(
            m_window
        );


        // ---------------------------------------------------------------------
        // Connect GLFW window with this Application instance.
        //
        // This allows static GLFW callbacks to recover the correct Application
        // object without any global variables.
        // ---------------------------------------------------------------------

        glfwSetWindowUserPointer(
            m_window,
            this
        );


        // ---------------------------------------------------------------------
        // Callbacks
        // ---------------------------------------------------------------------

        glfwSetFramebufferSizeCallback(
            m_window,
            framebuffer_size_callback
        );


        glfwSetMouseButtonCallback(
            m_window,
            mouse_button_callback
        );


        glfwSetCursorPosCallback(
            m_window,
            cursor_position_callback
        );


        // ---------------------------------------------------------------------
        // GLAD
        // ---------------------------------------------------------------------

        if (!gladLoadGL(
            glfwGetProcAddress
        ))
        {
            spdlog::error(
                "Failed to initialize GLAD"
            );

            return false;
        }


        m_is_opengl_initialized = true;


        // ---------------------------------------------------------------------
        // Framebuffer
        // ---------------------------------------------------------------------

        glfwGetFramebufferSize(
            m_window,
            &m_framebuffer_width,
            &m_framebuffer_height
        );


        glViewport(
            0,
            0,
            m_framebuffer_width,
            m_framebuffer_height
        );


        // ---------------------------------------------------------------------
        // OpenGL state
        // ---------------------------------------------------------------------

        glEnable(
            GL_DEPTH_TEST
        );


        // ---------------------------------------------------------------------
        // Geometry
        // ---------------------------------------------------------------------

        m_pyramid.create();


        // ---------------------------------------------------------------------
        // Shader
        // ---------------------------------------------------------------------

        if (!create_shader())
        {
            return false;
        }


        // ---------------------------------------------------------------------
        // Uniforms
        // ---------------------------------------------------------------------

        m_mvp_location =
            m_shader.get_uniform_location(
                "u_mvp"
            );


        if (m_mvp_location == -1)
        {
            spdlog::error(
                "Failed to find uniform location for u_mvp"
            );

            return false;
        }


        return true;
    }


    bool create_shader()
    {
        const std::filesystem::path vertex_shader_path =
            "../res/shader_program_sources/vertex.glsl";


        const std::filesystem::path fragment_shader_path =
            "../res/shader_program_sources/fragment.glsl";


        std::string vertex_shader_source;
        std::string fragment_shader_source;


        try
        {
            vertex_shader_source =
                andromeda::utils::FileOperations::load_file_as_string(
                    vertex_shader_path
                );


            fragment_shader_source =
                andromeda::utils::FileOperations::load_file_as_string(
                    fragment_shader_path
                );
        }
        catch (const std::exception& exception)
        {
            spdlog::error(
                "Failed to load shader files: {}",
                exception.what()
            );

            return false;
        }


        return m_shader.create(
            vertex_shader_source,
            fragment_shader_source
        );
    }


    // =========================================================================
    // Render loop
    // =========================================================================

    void render_loop()
    {
        while (!glfwWindowShouldClose(
            m_window
        ))
        {
            render();


            glfwSwapBuffers(
                m_window
            );


            glfwPollEvents();
        }
    }


    void render()
    {
        // ---------------------------------------------------------------------
        // Clear
        // ---------------------------------------------------------------------

        glClearColor(
            BACKGROUND_COLOR[0],
            BACKGROUND_COLOR[1],
            BACKGROUND_COLOR[2],
            BACKGROUND_COLOR[3]
        );


        glClear(
            GL_COLOR_BUFFER_BIT |
            GL_DEPTH_BUFFER_BIT
        );


        // ---------------------------------------------------------------------
        // Projection
        // ---------------------------------------------------------------------

        const glm::mat4 projection =
            create_projection_matrix();


        // ---------------------------------------------------------------------
        // View
        // ---------------------------------------------------------------------

        const glm::mat4 view =
            m_camera.get_view_matrix();


        // ---------------------------------------------------------------------
        // Model
        // ---------------------------------------------------------------------

        const glm::mat4 model =
            glm::scale(
                glm::mat4(1.0f),
                glm::vec3(
                    1.4f,
                    1.4f,
                    1.4f
                )
            );


        // ---------------------------------------------------------------------
        // MVP
        // ---------------------------------------------------------------------

        const glm::mat4 mvp =
            projection *
            view *
            model;


        // ---------------------------------------------------------------------
        // Shader
        // ---------------------------------------------------------------------

        m_shader.use();


        glUniformMatrix4fv(
            m_mvp_location,
            1,
            GL_FALSE,
            glm::value_ptr(
                mvp
            )
        );


        // ---------------------------------------------------------------------
        // Geometry
        // ---------------------------------------------------------------------

        m_pyramid.draw();
    }


    glm::mat4 create_projection_matrix() const
    {
        const int safe_height =
            m_framebuffer_height > 0
                ? m_framebuffer_height
                : 1;


        const float aspect_ratio =
            static_cast<float>(
                m_framebuffer_width
            )
            /
            static_cast<float>(
                safe_height
            );


        return glm::perspective(
            glm::radians(
                FOV_DEGREES_Y_AXIS
            ),
            aspect_ratio,
            NEAR_PLANE,
            FAR_PLANE
        );
    }


    // =========================================================================
    // Event handling
    // =========================================================================

    void on_framebuffer_size(
        int width,
        int height
    )
    {
        m_framebuffer_width = width;
        m_framebuffer_height = height;


        glViewport(
            0,
            0,
            width,
            height
        );
    }


    void on_mouse_button(
        int button,
        int action
    )
    {
        m_camera.on_mouse_button(
            button,
            action
        );
    }


    void on_cursor_position(
        double x,
        double y
    )
    {
        const bool is_ctrl_down =
            glfwGetKey(
                m_window,
                GLFW_KEY_LEFT_CONTROL
            ) == GLFW_PRESS
            ||
            glfwGetKey(
                m_window,
                GLFW_KEY_RIGHT_CONTROL
            ) == GLFW_PRESS;


        m_camera.on_mouse_move(
            x,
            y,
            is_ctrl_down
        );
    }


    // =========================================================================
    // GLFW callback bridge
    // =========================================================================

    static Application* get_application(
        GLFWwindow* window
    )
    {
        return static_cast<Application*>(
            glfwGetWindowUserPointer(
                window
            )
        );
    }


    static void framebuffer_size_callback(
        GLFWwindow* window,
        int width,
        int height
    )
    {
        Application* application =
            get_application(
                window
            );


        if (application)
        {
            application->on_framebuffer_size(
                width,
                height
            );
        }
    }


    static void mouse_button_callback(
        GLFWwindow* window,
        int button,
        int action,
        int mods
    )
    {
        Application* application =
            get_application(
                window
            );


        if (application)
        {
            application->on_mouse_button(
                button,
                action
            );
        }
    }


    static void cursor_position_callback(
        GLFWwindow* window,
        double x,
        double y
    )
    {
        Application* application =
            get_application(
                window
            );


        if (application)
        {
            application->on_cursor_position(
                x,
                y
            );
        }
    }


    // =========================================================================
    // Shutdown
    // =========================================================================

    void shutdown()
    {
        // ---------------------------------------------------------------------
        // OpenGL resources MUST be destroyed while the OpenGL context still
        // exists.
        // ---------------------------------------------------------------------

        if (m_is_opengl_initialized)
        {
            m_pyramid.destroy();
            m_shader.destroy();

            m_is_opengl_initialized = false;
        }


        // ---------------------------------------------------------------------
        // Window / OpenGL context
        // ---------------------------------------------------------------------

        if (m_window)
        {
            glfwDestroyWindow(
                m_window
            );

            m_window = nullptr;
        }


        // ---------------------------------------------------------------------
        // GLFW
        // ---------------------------------------------------------------------

        if (m_is_glfw_initialized)
        {
            glfwTerminate();

            m_is_glfw_initialized = false;
        }
    }


private:
    GLFWwindow* m_window = nullptr;


    bool m_is_glfw_initialized = false;
    bool m_is_opengl_initialized = false;


    int m_framebuffer_width = WINDOW_WIDTH;
    int m_framebuffer_height = WINDOW_HEIGHT;


    OrbitCamera m_camera;

    ShaderProgram m_shader;
    Pyramid m_pyramid;


    int m_mvp_location = -1;
};


int main()
{
    Application application;

    return application.run();
}