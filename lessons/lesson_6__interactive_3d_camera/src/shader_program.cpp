#include "shader_program.hpp"

#include "glad/gl.h"

#include "spdlog/spdlog.h"


ShaderProgram::~ShaderProgram()
{
    destroy();
}


bool ShaderProgram::create(
    const std::string& vertex_source,
    const std::string& fragment_source
)
{
    destroy();


    // -------------------------------------------------------------------------
    // Vertex shader
    // -------------------------------------------------------------------------

    const unsigned int vertex_shader =
        glCreateShader(
            GL_VERTEX_SHADER
        );


    if (!compile_shader(
        vertex_shader,
        vertex_source
    ))
    {
        glDeleteShader(
            vertex_shader
        );

        return false;
    }


    // -------------------------------------------------------------------------
    // Fragment shader
    // -------------------------------------------------------------------------

    const unsigned int fragment_shader =
        glCreateShader(
            GL_FRAGMENT_SHADER
        );


    if (!compile_shader(
        fragment_shader,
        fragment_source
    ))
    {
        glDeleteShader(
            vertex_shader
        );


        glDeleteShader(
            fragment_shader
        );


        return false;
    }


    // -------------------------------------------------------------------------
    // Program
    // -------------------------------------------------------------------------

    m_id =
        glCreateProgram();


    glAttachShader(
        m_id,
        vertex_shader
    );


    glAttachShader(
        m_id,
        fragment_shader
    );


    glLinkProgram(
        m_id
    );


    int success = 0;


    glGetProgramiv(
        m_id,
        GL_LINK_STATUS,
        &success
    );


    if (!success)
    {
        char info_log[512];


        glGetProgramInfoLog(
            m_id,
            sizeof(info_log),
            nullptr,
            info_log
        );


        spdlog::error(
            "Shader program linking failed: {}",
            info_log
        );


        glDeleteShader(
            vertex_shader
        );


        glDeleteShader(
            fragment_shader
        );


        destroy();

        return false;
    }


    // -------------------------------------------------------------------------
    // Individual shaders are no longer needed after linking.
    // -------------------------------------------------------------------------

    glDeleteShader(
        vertex_shader
    );


    glDeleteShader(
        fragment_shader
    );


    return true;
}


bool ShaderProgram::compile_shader(
    unsigned int shader_id,
    const std::string& source
)
{
    const char* source_c_str =
        source.c_str();


    glShaderSource(
        shader_id,
        1,
        &source_c_str,
        nullptr
    );


    glCompileShader(
        shader_id
    );


    int success = 0;


    glGetShaderiv(
        shader_id,
        GL_COMPILE_STATUS,
        &success
    );


    if (!success)
    {
        char info_log[512];


        glGetShaderInfoLog(
            shader_id,
            sizeof(info_log),
            nullptr,
            info_log
        );


        spdlog::error(
            "Shader compilation failed: {}",
            info_log
        );


        return false;
    }


    return true;
}


void ShaderProgram::destroy()
{
    if (m_id == 0)
    {
        return;
    }


    glDeleteProgram(
        m_id
    );


    m_id = 0;
}


void ShaderProgram::use() const
{
    glUseProgram(
        m_id
    );
}


int ShaderProgram::get_uniform_location(
    const char* name
) const
{
    return glGetUniformLocation(
        m_id,
        name
    );
}